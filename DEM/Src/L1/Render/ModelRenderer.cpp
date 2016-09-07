#include "ModelRenderer.h"

#include <Render/RenderFwd.h>
#include <Render/RenderNode.h>
#include <Render/Model.h>
#include <Render/Material.h>
#include <Render/Effect.h>
#include <Render/EffectConstSetValues.h>
#include <Render/Mesh.h>
#include <Render/Light.h>
#include <Render/GPUDriver.h>
#include <Math/Sphere.h>	//!!!for light testing only, refactor and optimize!
#include <Core/Factory.h>

namespace Render
{
__ImplementClass(Render::CModelRenderer, 'MDLR', Render::IRenderer);

CModelRenderer::CModelRenderer()
{
	// Setup dynamic enumeration
	InputSet_Model = RegisterShaderInputSetID(CStrID("Model"));
	InputSet_ModelSkinned = RegisterShaderInputSetID(CStrID("ModelSkinned"));
	InputSet_ModelInstanced = RegisterShaderInputSetID(CStrID("ModelInstanced"));

	InstanceDataDecl.SetSize(4);

	// World matrix
	for (U32 i = 0; i < 4; ++i)
	{
		CVertexComponent& Cmp = InstanceDataDecl[i];
		Cmp.Semantic = VCSem_TexCoord;
		Cmp.UserDefinedName = NULL;
		Cmp.Index = i + 4;
		Cmp.Format = VCFmt_Float32_4;
		Cmp.Stream = INSTANCE_BUFFER_STREAM_INDEX;
		Cmp.OffsetInVertex = DEM_VERTEX_COMPONENT_OFFSET_DEFAULT;
		Cmp.PerInstanceData = true;
	}

	//!!!DBG TMP! //???where to define?
	MaxInstanceCount = 30;
}
//---------------------------------------------------------------------

//???use object pos or AABB and closest point?!
//???!!!precalculate object-invariant values on light cache filling?!
// NB: always returns positive number.
static float CalcLightPriority(const vector3& ObjectPos, const vector3& LightPos, const vector3& LightInvDir, const CLight& Light)
{
	float SqIntensity = Light.Intensity * Light.Intensity;
	if (Light.Type == Light_Directional) return SqIntensity;

	float SqDistance = vector3::SqDistance(ObjectPos, LightPos);
	float Attenuation = (1.f - SqDistance * (Light.GetInvRange() * Light.GetInvRange()));

	if (Light.Type == Light_Spot && SqDistance != 0.f)
	{
		vector3 ModelLight = ObjectPos - LightPos;
		//ModelLight /= n_sqrt(SqDistance);
		ModelLight *= Math::RSqrt(SqDistance); //!!!TEST IT!
		float CosAlpha = ModelLight.Dot(LightInvDir);
		float Falloff = (CosAlpha - Light.GetCosHalfPhi()) / (Light.GetCosHalfTheta() - Light.GetCosHalfPhi());
		return SqIntensity * Attenuation * Clamp(Falloff, 0.f, 1.f);
	}

	return SqIntensity * Attenuation;
}
//---------------------------------------------------------------------

bool CModelRenderer::PrepareNode(CRenderNode& Node, const CRenderNodeContext& Context)
{
	CModel* pModel = Node.pRenderable->As<CModel>();
	n_assert_dbg(pModel);

	CMaterial* pMaterial = pModel->Material.GetUnsafe(); //!!!Get by MaterialLOD!
	if (!pMaterial) FAIL;

	CEffect* pEffect = pMaterial->GetEffect();
	EEffectType EffType = pEffect->GetType();
	if (Context.pEffectOverrides)
		for (UPTR i = 0; i < Context.pEffectOverrides->GetCount(); ++i)
			if (Context.pEffectOverrides->KeyAt(i) == EffType)
			{
				pEffect = Context.pEffectOverrides->ValueAt(i).GetUnsafe();
				break;
			}

	if (!pEffect) FAIL;

	Node.pMaterial = pMaterial;
	Node.pEffect = pEffect;
	Node.pTech = pEffect->GetTechByInputSet(Node.pSkinPalette ? InputSet_ModelSkinned : InputSet_Model);
	if (!Node.pTech) FAIL;

	Node.pMesh = pModel->Mesh.GetUnsafe();
	Node.pGroup = pModel->Mesh->GetGroup(pModel->MeshGroupIndex, Context.MeshLOD);

	if (pModel->BoneIndices.GetCount())
	{
		Node.pSkinMapping = pModel->BoneIndices.GetPtr();
		Node.BoneCount = pModel->BoneIndices.GetCount();
	}
	else Node.pSkinMapping = NULL;

	U8 LightCount = 0;

	if (Context.pLights && MAX_LIGHT_COUNT_PER_OBJECT)
	{
		n_assert_dbg(Context.pLightIndices);

		CArray<U16>& LightIndices = *Context.pLightIndices;
		Node.LightIndexBase = LightIndices.GetCount();

		float LightPriority[MAX_LIGHT_COUNT_PER_OBJECT];
		bool LightOverflow = false;

		const CArray<CLightRecord>& Lights = *Context.pLights;
		for (UPTR i = 0; i < Lights.GetCount(); ++i)
		{
			CLightRecord& LightRec = Lights[i];
			const CLight* pLight = LightRec.pLight;
			switch (pLight->Type)
			{
				case Light_Point:
				{
					//!!!???avoid object creation, rewrite functions so that testing against vector + float is possible!?
					sphere LightBounds(LightRec.Transform.Translation(), pLight->GetRange());
					if (!LightBounds.GetClipStatus(Context.AABB)) continue;
					break;
				}
				case Light_Spot:
				{
					//!!!???PERF: test against sphere before?!
					//???cache GlobalFrustum in a light record?
					matrix44 LocalFrustum;
					pLight->CalcLocalFrustum(LocalFrustum);
					matrix44 GlobalFrustum;
					LightRec.Transform.invert_simple(GlobalFrustum);
					GlobalFrustum *= LocalFrustum;
					if (!Context.AABB.GetClipStatus(GlobalFrustum)) continue;
					break;
				}
			}

			if (LightCount < MAX_LIGHT_COUNT_PER_OBJECT)
			{				
				LightIndices.Add(i);
				++LightCount;
				++LightRec.UseCount;
			}
			else
			{
				if (!LightOverflow)
				{
					// Calculate light priorities for already collected lights
					for (U16 j = 0; j < LightCount; ++j)
					{
						const CLightRecord& CurrRec = Lights[LightIndices[Node.LightIndexBase + j]];
						LightPriority[j] = CalcLightPriority(
							Node.Transform.Translation(),
							CurrRec.Transform.Translation(),
							CurrRec.Transform.AxisZ(),
							*pLight);
					}

					LightOverflow = true;
				}

				float CurrLightPriority = CalcLightPriority(
					Node.Transform.Translation(),
					LightRec.Transform.Translation(),
					LightRec.Transform.AxisZ(),
					*pLight);

				//PERF: faster but less precise approach is to replace the first light with lower priority
				float MinPriority = LightPriority[0];
				U16 MinPriorityIndex = 0;
				for (U16 j = 1; j < LightCount; ++j)
				{
					if (LightPriority[j] < MinPriority)
					{
						MinPriority = LightPriority[j];
						MinPriorityIndex = j;
					}
				}
				
				if (CurrLightPriority > MinPriority)
				{
					U16& LightIndex = LightIndices[Node.LightIndexBase + MinPriorityIndex];
					--Lights[LightIndex].UseCount;

					LightIndex = i;
					LightPriority[MinPriorityIndex] = CurrLightPriority;
					++LightRec.UseCount;
				}

				//break; // - if don't want to calculate priorities, just skip all remaining lights
			}
		}
	}

	Node.LightCount = LightCount;

	OK;
}
//---------------------------------------------------------------------

// Optimal sorting for the color phase is Tech-Material-Mesh-Group for opaque and then BtF for transparent.
// Tech is sorted before Material because it is more likely that many materials will be rendered with the same
// single-pass tech, than that the same material will be used with many different techs. We have great chances
// to set render state only once as our tech is single-pass, and to render many materials without switching it,
// just rebinding constants, resources and samplers.
CArray<CRenderNode*>::CIterator CModelRenderer::Render(const CRenderContext& Context,
													   CArray<CRenderNode*>& RenderQueue,
													   CArray<CRenderNode*>::CIterator ItCurr)
{
	CGPUDriver& GPU = *Context.pGPU;

	const CMaterial* pCurrMaterial = NULL;
	const CMesh* pCurrMesh = NULL;
	const CTechnique* pCurrTech = NULL;
	CVertexLayout* pVL = NULL;
	CVertexLayout* pVLInstanced = NULL;

	const CEffectConstant* pConstWorldMatrix = NULL;	// Model, ModelSkinned
	const CEffectConstant* pConstSkinPalette = NULL;	// ModelSkinned
	const CEffectConstant* pConstInstanceData = NULL;	// ModelInstanced

	CArray<CRenderNode*>::CIterator ItEnd = RenderQueue.End();
	while (ItCurr != ItEnd)
	{
		CRenderNode* pRenderNode = *ItCurr;

		if (pRenderNode->pRenderer != this) return ItCurr;

		const CTechnique* pTech = pRenderNode->pTech;
		const CPrimitiveGroup* pGroup = pRenderNode->pGroup;
		n_assert_dbg(pGroup && pTech);

		// Apply material, if changed

		const CMaterial* pMaterial = pRenderNode->pMaterial;
		if (pMaterial != pCurrMaterial)
		{
			n_assert_dbg(pMaterial);
			n_verify_dbg(pMaterial->Apply(GPU));
			pCurrMaterial = pMaterial;

			//!!!DBG TMP!
			//Sys::DbgOut("Material changed: 0x%X\n", pMaterial);
		}

		// Apply geometry, if changed

		const CMesh* pMesh = pRenderNode->pMesh;
		if (pMesh != pCurrMesh)
		{
			n_assert_dbg(pMesh);
			CVertexBuffer* pVB = pMesh->GetVertexBuffer().GetUnsafe();
			n_assert_dbg(pVB);
			GPU.SetVertexBuffer(0, pVB);
			GPU.SetIndexBuffer(pMesh->GetIndexBuffer().GetUnsafe());
			pCurrMesh = pMesh;

			pVL = pVB->GetVertexLayout();
			pVLInstanced = NULL;

			//!!!DBG TMP!
			//Sys::DbgOut("Mesh changed: 0x%X\n", pMesh);
		}

		// Select corresponding tech variation

		UPTR LightCount = pRenderNode->LightCount;
		//!!!calc lights!
		//for instances may select maximum of light counts and use black lights for ones not used, or use per-instance count and dynamic loop
		//tech with a dynamic light count will be found at LightCount = 0

		// Gather instances (no skinned instancing supported)

		bool HardwareInstancing = false;
		CArray<CRenderNode*>::CIterator ItInstEnd = ItCurr + 1;
		if (!pRenderNode->pSkinPalette)
		{
			while (ItInstEnd != ItEnd &&
				   (*ItInstEnd)->pRenderer == this &&
				   (*ItInstEnd)->pMaterial == pMaterial &&
				   (*ItInstEnd)->pTech == pTech &&
				   (*ItInstEnd)->pGroup == pGroup &&
				   !(*ItInstEnd)->pSkinPalette)
			{
				// We don't try to find an instanced tech version here, and don't break if
				// it is not found, because if we did, the next object will try to do all
				// this again, not knowing that there is no chance to success. If there is
				// no instanced tech version, we render instances in a loop manually instead.
				++ItInstEnd;
			}

			if (ItInstEnd - ItCurr > 1)
			{
				const CTechnique* pInstancedTech = pRenderNode->pEffect->GetTechByInputSet(InputSet_ModelInstanced);
				if (pInstancedTech)
				{
					pTech = pInstancedTech;
					HardwareInstancing = true;
				}
			}
		}

		// Select tech variation for the current instancing mode and light count

		const CPassList* pPasses = pTech->GetPasses(LightCount);
		n_assert_dbg(pPasses); // To test if it could happen at all
		if (!pPasses)
		{
			ItCurr = ItInstEnd;
			continue;
		}

		if (HardwareInstancing)
		{
			if (pTech != pCurrTech)
			{
				pConstInstanceData = pTech->GetConstant(CStrID("InstanceData"));
				pCurrTech = pTech;

				//!!!DBG TMP!
				//Sys::DbgOut("Tech params requested by ID\n");
			}

			if (pConstInstanceData)
			{
				UPTR MaxInstanceCountConst = pConstInstanceData->Desc.ElementCount;
				n_assert_dbg(MaxInstanceCountConst > 1);

				//!!!DBG TMP!
				if ((ItInstEnd - ItCurr) > (IPTR)MaxInstanceCountConst)
					Sys::DbgOut("Instance buffer overflow (%d of %d), data will be split\n", (ItInstEnd - ItCurr), MaxInstanceCountConst);

				GPU.SetVertexLayout(pVL);

				CEffectConstSetValues PerInstanceConstValues;
				PerInstanceConstValues.SetGPU(&GPU);
				PerInstanceConstValues.RegisterConstantBuffer(pConstInstanceData->Desc.BufferHandle, NULL);

				UPTR InstanceCount = 0;
				while (ItCurr != ItInstEnd)
				{
					PerInstanceConstValues.SetConstantValue(pConstInstanceData, InstanceCount, pRenderNode->Transform.m, sizeof(matrix44));
					++InstanceCount;
					++ItCurr;
					pRenderNode = *ItCurr;

					if (InstanceCount == MaxInstanceCountConst)
					{
						PerInstanceConstValues.ApplyConstantBuffers();
						for (UPTR i = 0; i < pPasses->GetCount(); ++i)
						{
							GPU.SetRenderState((*pPasses)[i]);
							GPU.DrawInstanced(*pGroup, InstanceCount);
						}
						InstanceCount = 0;
						if (ItCurr == ItInstEnd) break;
					}
				}

				if (InstanceCount)
				{
					PerInstanceConstValues.ApplyConstantBuffers();
					for (UPTR i = 0; i < pPasses->GetCount(); ++i)
					{
						GPU.SetRenderState((*pPasses)[i]);
						GPU.DrawInstanced(*pGroup, InstanceCount);
					}
				}
			}
			else
			{
				n_assert_dbg(MaxInstanceCount > 1);

				//!!!DBG TMP!
				if ((ItInstEnd - ItCurr) > (IPTR)MaxInstanceCount)
					Sys::DbgOut("Instance buffer overflow (%d of %d), data will be split\n", (ItInstEnd - ItCurr), MaxInstanceCount);

				// We create this buffer lazy because for D3D11 possibility is high to use only constant-based instancing
				if (InstanceVB.IsNullPtr())
				{
					PVertexLayout VLInstanceData = GPU.CreateVertexLayout(InstanceDataDecl.GetPtr(), InstanceDataDecl.GetCount());
					InstanceVB = GPU.CreateVertexBuffer(*VLInstanceData, MaxInstanceCount, Access_CPU_Write | Access_GPU_Read);
				}

				if (!pVLInstanced)
				{
					IPTR VLIdx = InstancedLayouts.FindIndex(pVL);
					if (VLIdx == INVALID_INDEX)
					{
						UPTR BaseComponentCount = pVL->GetComponentCount();
						UPTR DescComponentCount = BaseComponentCount + InstanceDataDecl.GetCount();
						CVertexComponent* pInstancedDecl = (CVertexComponent*)_malloca(DescComponentCount * sizeof(CVertexComponent));
						memcpy(pInstancedDecl, pVL->GetComponent(0), BaseComponentCount * sizeof(CVertexComponent));
						memcpy(pInstancedDecl + BaseComponentCount, InstanceDataDecl.GetPtr(), InstanceDataDecl.GetCount() * sizeof(CVertexComponent));

						PVertexLayout VLInstanced = GPU.CreateVertexLayout(pInstancedDecl, DescComponentCount);

						_freea(pInstancedDecl);

						pVLInstanced = VLInstanced.GetUnsafe();
						n_assert_dbg(pVLInstanced);
						InstancedLayouts.Add(pVL, VLInstanced);
					}
					else pVLInstanced = InstancedLayouts.ValueAt(VLIdx).GetUnsafe();
				}

				GPU.SetVertexLayout(pVLInstanced);
				GPU.SetVertexBuffer(INSTANCE_BUFFER_STREAM_INDEX, InstanceVB.GetUnsafe());

				void* pInstData;
				n_verify(GPU.MapResource(&pInstData, *InstanceVB, Map_WriteDiscard)); //???use big buffer + no overwrite?
				UPTR InstanceCount = 0;
				while (ItCurr != ItInstEnd)
				{
					memcpy(pInstData, pRenderNode->Transform.m, sizeof(matrix44));
					pInstData = (char*)pInstData + sizeof(matrix44);
					++InstanceCount;
					++ItCurr;
					pRenderNode = *ItCurr;

					if (InstanceCount == MaxInstanceCount)
					{
						GPU.UnmapResource(*InstanceVB);
						for (UPTR i = 0; i < pPasses->GetCount(); ++i)
						{
							GPU.SetRenderState((*pPasses)[i]);
							GPU.DrawInstanced(*pGroup, InstanceCount);
						}
						InstanceCount = 0;
						if (ItCurr == ItInstEnd) break;
						n_verify(GPU.MapResource(&pInstData, *InstanceVB, Map_WriteDiscard)); //???use big buffer + no overwrite?
					}
				}

				if (InstanceCount)
				{
					GPU.UnmapResource(*InstanceVB);
					for (UPTR i = 0; i < pPasses->GetCount(); ++i)
					{
						GPU.SetRenderState((*pPasses)[i]);
						GPU.DrawInstanced(*pGroup, InstanceCount);
					}
				}
			}

			//Sys::DbgOut("CModel rendered instanced, tech '%s', group 0x%X, instances: %d\n", pTech->GetName().CStr(), pGroup, (ItInstEnd - ItCurr));
		}
		else
		{
			if (pTech != pCurrTech)
			{
				pConstWorldMatrix = pTech->GetConstant(CStrID("WorldMatrix"));
				pConstSkinPalette = pTech->GetConstant(CStrID("SkinPalette"));
				pCurrTech = pTech;

				//!!!DBG TMP!
				//Sys::DbgOut("Tech params requested by ID\n");
			}

			GPU.SetVertexLayout(pVL);

			for (; ItCurr != ItInstEnd; ++ItCurr, pRenderNode = *ItCurr)
			{
				// Per-instance params

				//???use persistent, create once and store associatively Tech->Values?
				CEffectConstSetValues PerInstanceConstValues;
				PerInstanceConstValues.SetGPU(&GPU);

				if (pConstWorldMatrix)
				{
					PerInstanceConstValues.RegisterConstantBuffer(pConstWorldMatrix->Desc.BufferHandle, NULL);
					PerInstanceConstValues.SetConstantValue(pConstWorldMatrix, 0, pRenderNode->Transform.m, sizeof(matrix44));
				}

				if (pConstSkinPalette && pRenderNode->pSkinPalette)
				{
					UPTR BoneCount = n_min(pRenderNode->BoneCount, pConstSkinPalette->Desc.ElementCount);

					PerInstanceConstValues.RegisterConstantBuffer(pConstSkinPalette->Desc.BufferHandle, NULL);
					if (pConstSkinPalette->Desc.Flags & Const_ColumnMajor) //???hide in a class/function?
					{
						// Transpose and truncate if necessary
						U32 Columns = pConstSkinPalette->Desc.Columns;
						U32 Rows = pConstSkinPalette->Desc.Rows;
						UPTR MatrixSize = Columns * Rows * sizeof(float);
						float* pTransposedData = (float*)_malloca(MatrixSize);

						if (pRenderNode->pSkinMapping)
						{
							for (UPTR BoneIdxIdx = 0; BoneIdxIdx < BoneCount; ++BoneIdxIdx)
							{
								float* pCurrData = pTransposedData;
								const matrix44* pBoneMatrix = pRenderNode->pSkinPalette + pRenderNode->pSkinMapping[BoneIdxIdx];
								for (U32 Col = 0; Col < Columns; ++Col)
									for (U32 Row = 0; Row < Rows; ++Row)
									{
										*pCurrData = pBoneMatrix->m[Row][Col];
										++pCurrData;
									}
								PerInstanceConstValues.SetConstantValue(pConstSkinPalette, BoneIdxIdx, pTransposedData, MatrixSize);
							}
						}
						else
						{
							for (UPTR BoneIdx = 0; BoneIdx < BoneCount; ++BoneIdx)
							{
								float* pCurrData = pTransposedData;
								const matrix44* pBoneMatrix = pRenderNode->pSkinPalette + BoneIdx;
								for (U32 Col = 0; Col < Columns; ++Col)
									for (U32 Row = 0; Row < Rows; ++Row)
									{
										*pCurrData = pBoneMatrix->m[Row][Col];
										++pCurrData;
									}
								PerInstanceConstValues.SetConstantValue(pConstSkinPalette, BoneIdx, pTransposedData, MatrixSize);
							}
						}

						_freea(pTransposedData);
					}
					else
					{
						if (pRenderNode->pSkinMapping)
						{
							for (UPTR BoneIdxIdx = 0; BoneIdxIdx < BoneCount; ++BoneIdxIdx)
							{
								const matrix44* pBoneMatrix = pRenderNode->pSkinPalette + pRenderNode->pSkinMapping[BoneIdxIdx];
								PerInstanceConstValues.SetConstantValue(pConstSkinPalette, BoneIdxIdx, pBoneMatrix, sizeof(matrix44));
							}
						}
						else
						{
							PerInstanceConstValues.SetConstantValue(pConstSkinPalette, 0, pRenderNode->pSkinPalette, sizeof(matrix44) * BoneCount);
						}
					}
				}

				PerInstanceConstValues.ApplyConstantBuffers();

				// Rendering

				//???loop by pass, then by instance? possibly less render state switches, but possibly more data binding. Does order matter?
				for (UPTR i = 0; i < pPasses->GetCount(); ++i)
				{
					GPU.SetRenderState((*pPasses)[i]);
					GPU.Draw(*pGroup);
				}

				//Sys::DbgOut("CModel rendered non-instanced, tech '%s', group 0x%X, primitives: %d\n", pTech->GetName().CStr(), pGroup, pGroup->IndexCount);
			}
		}

	};

	return ItEnd;
}
//---------------------------------------------------------------------

}