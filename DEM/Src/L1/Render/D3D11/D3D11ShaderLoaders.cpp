#include "D3D11ShaderLoaders.h"

#include <Resources/Resource.h>
#include <Render/D3D11/D3D11DriverFactory.h>
#include <Render/D3D11/D3D11GPUDriver.h>
#include <Render/D3D11/D3D11Shader.h>
#include <IO/IOServer.h>
#include <IO/BinaryReader.h>
#include <IO/Streams/FileStream.h>
#include <Data/StringUtils.h>
#include <Core/Factory.h>
#define WIN32_LEAN_AND_MEAN
#include <d3d11.h>
#undef GetObject

namespace Resources
{
__ImplementClass(Resources::CD3D11VertexShaderLoader, 'VSL1', Resources::CD3D11ShaderLoader);
__ImplementClass(Resources::CD3D11PixelShaderLoader, 'PSL1', Resources::CD3D11ShaderLoader);

///////////////////////////////////////////////////////////////////////
// CD3D11ShaderLoader
///////////////////////////////////////////////////////////////////////

const Core::CRTTI& CD3D11ShaderLoader::GetResultType() const
{
	return Render::CD3D11Shader::RTTI;
}
//---------------------------------------------------------------------

bool CD3D11ShaderLoader::LoadImpl(CResource& Resource, Render::EShaderType ShaderType)
{
	if (GPU.IsNullPtr() || !GPU->IsA<Render::CD3D11GPUDriver>()) FAIL;

	//!!!some streams don't support Seek and GetSize!
	IO::PStream IOStream = IOSrv->CreateStream(Resource.GetUID().CStr());
	if (!IOStream->Open(IO::SAM_READ, IO::SAP_RANDOM)) FAIL;
	U64 FileSize = IOStream->GetSize();

	IO::CBinaryReader R(*IOStream);

	Data::CFourCC FileSig;
	if (!R.Read(FileSig)) FAIL;

	switch (ShaderType)
	{
		//???separate type and target? type =, target >=
		case Render::ShaderType_Vertex:		if (FileSig != 'VS40' && FileSig != 'VS41' && FileSig != 'VS50') FAIL; break;
		case Render::ShaderType_Pixel:		if (FileSig != 'PS40' && FileSig != 'PS41' && FileSig != 'PS50') FAIL; break;
		case Render::ShaderType_Geometry:	if (FileSig != 'GS40' && FileSig != 'GS41' && FileSig != 'GS50') FAIL; break;
		case Render::ShaderType_Hull:		if (FileSig != 'HS50') FAIL; break;
		case Render::ShaderType_Domain:		if (FileSig != 'DS50') FAIL; break;
		default:
		{
			// Shader type autodetection
			switch (FileSig.Code)
			{
				case 'VS40':
				case 'VS41':
				case 'VS50':	ShaderType = Render::ShaderType_Vertex; break;
				case 'PS40':
				case 'PS41':
				case 'PS50':	ShaderType = Render::ShaderType_Pixel; break;
				case 'GS40':
				case 'GS41':
				case 'GS50':	ShaderType = Render::ShaderType_Geometry; break;
				case 'HS50':	ShaderType = Render::ShaderType_Hull; break;
				case 'DS50':	ShaderType = Render::ShaderType_Domain; break;
				default:		FAIL;
			};
			break;
		}
	}

	U32 BinaryOffset;
	if (!R.Read(BinaryOffset)) FAIL;

	U32 ShaderFileID;
	if (!R.Read(ShaderFileID)) FAIL;

	U32 InputSignatureID;
	if (!R.Read(InputSignatureID)) FAIL;

	U64 MetadataOffset = IOStream->GetPosition();

	UPTR BinarySize = (UPTR)FileSize - (UPTR)BinaryOffset;
	if (!BinarySize) FAIL;
	void* pData = n_malloc(BinarySize);
	if (!pData) FAIL;
	if (!IOStream->Seek(BinaryOffset, IO::Seek_Begin) || IOStream->Read(pData, BinarySize) != BinarySize)
	{
		n_free(pData);
		FAIL;
	}

	void* pSigData = NULL;
	if (ShaderType == Render::ShaderType_Vertex)
	{
		if (InputSignatureID)
		{
			// StrID = URI formed with InputSignatureID
		}
		else
		{
			InputSignatureID = ShaderFileID;
			// StrID = Shader URI
		}

		if (!D3D11DrvFactory->FindShaderInputSignature(InputSignatureID))
		{
			UPTR SigSize;
			if (InputSignatureID == ShaderFileID)
			{
				pSigData = pData;
				SigSize = BinarySize;
			}
			else
			{
				const char pDir[] = "Shaders:Bin/";
				CString Path(pDir, sizeof(pDir) - 1, 32);
				Path += StringUtils::FromInt(InputSignatureID);
				Path += ".sig";
				IO::PStream SigFile = IOSrv->CreateStream(Path.CStr());
				if (!SigFile->Open(IO::SAM_READ, IO::SAP_SEQUENTIAL))
				{
					n_free(pData);
					FAIL;
				}
				SigSize = (UPTR)SigFile->GetSize();
				pSigData = n_malloc(SigSize);
				if (SigFile->Read(pSigData, SigSize) != SigSize)
				{
					n_free(pData);
					n_free(pSigData);
					FAIL;
				}
			}

			if (!D3D11DrvFactory->RegisterShaderInputSignature(InputSignatureID, pSigData, SigSize))
			{
				n_free(pData);
				n_free(pSigData);
				FAIL;
			}
		}
	}

	Render::PD3D11Shader Shader = (Render::CD3D11Shader*)GPU->CreateShader(ShaderType, pData, BinarySize).GetUnsafe();
	if (Shader.IsNullPtr()) FAIL;

	if (pSigData != pData) n_free(pData);

	Shader->InputSignatureID = InputSignatureID;

	if (!IOStream->Seek(MetadataOffset, IO::Seek_Begin)) FAIL;

	//???where to validate? will be loaded at all? mb load and check these fields before creating D3D API shader object?
	U32 MinFeatureLevelValue;
	R.Read<U32>(MinFeatureLevelValue);
	Shader->MinFeatureLevel = (D3D_FEATURE_LEVEL)MinFeatureLevelValue;

	R.Read<U64>(Shader->RequiresFlags);

	CFixedArray<Render::CD3D11Shader::CBufferMeta>& Buffers = Shader->Buffers;
	Buffers.SetSize(R.Read<U32>());
	for (UPTR i = 0; i < Buffers.GetCount(); ++i)
	{
		Render::CD3D11Shader::CBufferMeta* pMeta = &Buffers[i];
		if (!R.Read(pMeta->Name)) return NULL;
		if (!R.Read<U32>(pMeta->Register)) return NULL;

		U32 BufType = pMeta->Register >> 30;
		switch (BufType)
		{
			case 0:		pMeta->Type = Render::CD3D11Shader::ConstantBuffer; break;
			case 1:		pMeta->Type = Render::CD3D11Shader::TextureBuffer; break;
			case 2:		pMeta->Type = Render::CD3D11Shader::StructuredBuffer; break;
			default:	FAIL;
		};

		pMeta->Register &= 0x3fffffff; // Clear bits 30 and 31

		if (!R.Read<U32>(pMeta->Size)) return NULL;

		// For non-empty buffers open handles at the load time to reference buffers from constants
		pMeta->Handle = pMeta->Size ? D3D11DrvFactory->HandleMgr.OpenHandle(pMeta) : INVALID_HANDLE;
	}

	CFixedArray<Render::CD3D11Shader::CConstMeta>& Consts = Shader->Consts;
	Consts.SetSize(R.Read<U32>());
	for (UPTR i = 0; i < Consts.GetCount(); ++i)
	{
		Render::CD3D11Shader::CConstMeta* pMeta = &Consts[i];
		if (!R.Read(pMeta->Name)) return NULL;

		U32 BufIdx;
		if (!R.Read(BufIdx)) return NULL;
		pMeta->BufferHandle = Buffers[BufIdx].Handle;

		U8 Type;
		if (!R.Read<U8>(Type)) return NULL;
		pMeta->Type = (Render::CD3D11Shader::EConstType)Type;

		if (!R.Read<U32>(pMeta->Offset)) return NULL;
		if (!R.Read<U32>(pMeta->ElementSize)) return NULL;
		if (!R.Read<U32>(pMeta->ElementCount)) return NULL;

		pMeta->Handle = INVALID_HANDLE;
	}

	CFixedArray<Render::CD3D11Shader::CRsrcMeta>& Resources = Shader->Resources;
	Resources.SetSize(R.Read<U32>());
	for (UPTR i = 0; i < Resources.GetCount(); ++i)
	{
		Render::CD3D11Shader::CRsrcMeta* pMeta = &Resources[i];
		if (!R.Read(pMeta->Name)) return NULL;

		U8 Type;
		if (!R.Read<U8>(Type)) return NULL;
		pMeta->Type = (Render::CD3D11Shader::ERsrcType)Type;

		if (!R.Read<U32>(pMeta->RegisterStart)) return NULL;
		if (!R.Read<U32>(pMeta->RegisterCount)) return NULL;

		pMeta->Handle = INVALID_HANDLE;
	}

	CFixedArray<Render::CD3D11Shader::CSamplerMeta>& Samplers = Shader->Samplers;
	Samplers.SetSize(R.Read<U32>());
	for (UPTR i = 0; i < Samplers.GetCount(); ++i)
	{
		Render::CD3D11Shader::CSamplerMeta* pMeta = &Samplers[i];
		if (!R.Read(pMeta->Name)) return NULL;
		if (!R.Read<U32>(pMeta->RegisterStart)) return NULL;
		if (!R.Read<U32>(pMeta->RegisterCount)) return NULL;

		pMeta->Handle = INVALID_HANDLE;
	}

	Resource.Init(Shader.GetUnsafe(), this);

	OK;
}
//---------------------------------------------------------------------

///////////////////////////////////////////////////////////////////////
// CD3D11VertexShaderLoader
///////////////////////////////////////////////////////////////////////

bool CD3D11VertexShaderLoader::IsProvidedDataValid() const
{
	if (GPU.IsNullPtr() || !GPU->IsA<Render::CD3D11GPUDriver>()) FAIL;

	//!!!can read raw data to IO cache! or, if stream is mapped, can open and close stream outside these methods!
	//GPU->ValidateShaderBinary(ShaderType);
	//	return SUCCEEDED(pGPU->GetD3DDevice()->CreateVertexShader(buf, buflen, NULL, NULL));

	OK;
}
//---------------------------------------------------------------------

///////////////////////////////////////////////////////////////////////
// CD3D11PixelShaderLoader
///////////////////////////////////////////////////////////////////////

bool CD3D11PixelShaderLoader::IsProvidedDataValid() const
{
	if (GPU.IsNullPtr() || !GPU->IsA<Render::CD3D11GPUDriver>()) FAIL;

	//!!!can read raw data to IO cache! or, if stream is mapped, can open and close stream outside these methods!
	//GPU->ValidateShaderBinary(ShaderType);
	//	return SUCCEEDED(pGPU->GetD3DDevice()->CreatePixelShader(buf, buflen, NULL, NULL));

	OK;
}
//---------------------------------------------------------------------

}