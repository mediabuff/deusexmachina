#pragma once
#ifndef __DEM_L1_RENDER_SERVER_H__
#define __DEM_L1_RENDER_SERVER_H__

#include <Render/Display.h>
#include <Render/Materials/Material.h>
#include <Render/Materials/Texture.h>
#include <Render/RenderTarget.h>
#include <Render/Geometry/Mesh.h>
#include <Resources/ResourceManager.h>
#include <Data/Data.h>

// Render device interface (currently D3D9). Renderer manages shaders, shader state, shared variables,
// render targets, model, view and projection transforms, and performs actual rendering

//!!!need FrameLog - logging of rendering calls for one particular frame!

namespace Render
{
#define RenderSrv Render::CRenderServer::Instance()

class CRenderServer: public Core::CRefCounted
{
	DeclareRTTI;
	__DeclareSingleton(CRenderServer);

public:

	enum
	{
		MaxTextureStageCount = 8,
		MaxRenderTargetCount = 4,
		MaxVertexStreamCount = 2 // Not sure why 2, N3 value
	};

protected:

	//!!!OLD!
	friend class nD3D9Server;

	enum { MaxShaderFeatureCount = sizeof(DWORD) * 8 };

	bool								_IsOpen;
	bool								IsInsideFrame;
	DWORD								InstanceCount;	// If 0, non-instanced rendering is active

	nDictionary<CStrID, PVertexLayout>	VertexLayouts;
	nDictionary<CStrID, int>			ShaderFeatures;

	CDisplay							Display;
	PRenderTarget						DefaultRT;

	DWORD								FrameID;
	PRenderTarget						CurrRT[MaxRenderTargetCount];
	PVertexBuffer						CurrVB[MaxVertexStreamCount];
	DWORD								CurrVBOffset[MaxVertexStreamCount];
	PVertexLayout						CurrVLayout;
	PIndexBuffer						CurrIB;
	CMeshGroup							CurrPrimGroup;
	EPixelFormat						CurrDepthStencilFormat;

	UINT								D3DAdapter;
	D3DPRESENT_PARAMETERS				D3DPresentParams;
	IDirect3D9*							pD3D;
	IDirect3DDevice9*					pD3DDevice;
	ID3DXEffectPool*					pEffectPool;

	bool				CreateDevice();
	void				ReleaseDevice();
	void				SetupBufferFormats();

public:

	Resources::CResourceManager<CMesh>		MeshMgr;
	Resources::CResourceManager<CTexture>	TextureMgr;
	Resources::CResourceManager<CShader>	ShaderMgr;
	Resources::CResourceManager<CMaterial>	MaterialMgr;

	CRenderServer();
	~CRenderServer() { __DestructSingleton; }

	bool				Open();
	void				Close();
	bool				IsOpen() const { return _IsOpen; }
	void				ResetDevice(); // If display size changed or device lost

	bool				BeginFrame();
	void				EndFrame();
	void				Present();
	void				SaveScreenshot(EImageFormat ImageFormat, Data::CStream& OutStream);

	//!!!set transforms (MVP, Light etc)! here or where? mb in passes?
	void				SetRenderTarget(DWORD Index, CRenderTarget* pRT);
	void				SetVertexBuffer(DWORD Index, CVertexBuffer* pVB, DWORD OffsetVertex = 0);
	void				SetVertexLayout(CVertexLayout* pVLayout);
	void				SetIndexBuffer(CIndexBuffer* pIB);
	void				SetInstanceBuffer(DWORD Index, CVertexBuffer* pVB, DWORD Instances, DWORD OffsetVertex = 0);
	void				SetPrimitiveGroup(CMeshGroup& Group) { CurrPrimGroup = Group; }
	void				Clear(DWORD Flags, DWORD Color, float Depth, uchar Stencil);
	void				Draw();

	PVertexLayout		GetVertexLayout(const nArray<CVertexComponent>& Components);
	DWORD				ShaderFeatureStringToMask(const nString& FeatureString);
	DWORD				CalcPrimitiveCount(EPrimitiveTopology Topology, DWORD VertexCount);

	CDisplay&			GetDisplay() { return Display; }
	DWORD				GetFrameID() const { return FrameID; }
	UINT				GetD3DAdapter() const { return D3DAdapter; }
	IDirect3D9*			GetD3D() const { return pD3D; } //!!!static method in N3, creates D3D!
	IDirect3DDevice9*	GetD3DDevice() const { return pD3DDevice; }
	ID3DXEffectPool*	GetD3DEffectPool() const { return pEffectPool; }
};

inline CRenderServer::CRenderServer():
	_IsOpen(false),
	IsInsideFrame(false),
	InstanceCount(0),
	FrameID(0),
	pD3D(NULL),
	pD3DDevice(NULL),
	pEffectPool(NULL),
	CurrDepthStencilFormat(PixelFormat_Invalid)
{
	__ConstructSingleton;
	memset(CurrVBOffset, 0, sizeof(CurrVBOffset));
}
//---------------------------------------------------------------------

}

#endif
