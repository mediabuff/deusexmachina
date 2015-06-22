#pragma once
#ifndef __DEM_L1_RENDER_D3D9_GPU_DRIVER_H__
#define __DEM_L1_RENDER_D3D9_GPU_DRIVER_H__

#include <Render/GPUDriver.h>
#include <Render/D3D9/D3D9SwapChain.h>

#define WIN32_LEAN_AND_MEAN
#define D3D_DISABLE_9EX
#include <d3d9.h> // At least for a CAPS structure

// Direct3D9 GPU device driver.
// Multihead (multimonitor) feature is not implemented. You may do it by yourself.
// NB: D3D9 device can't be created without a swap chain, so you MUST call CreateSwapChain()
// before using any device-dependent methods.

namespace Render
{

class CD3D9GPUDriver: public CGPUDriver
{
	__DeclareClass(CD3D9GPUDriver);

protected:

	CArray<CD3D9SwapChain>	SwapChains;
	bool					IsInsideFrame;
	//bool					Wireframe;

	D3DCAPS9				D3DCaps;
	IDirect3DDevice9*		pD3DDevice;

	Events::PSub			Sub_OnPaint;

	CD3D9GPUDriver(): SwapChains(1, 1), pD3DDevice(NULL), IsInsideFrame(false) {}

	//???subscribe swapchain itself? more handling and self-control into a swapchain class?
	bool				OnOSWindowToggleFullscreen(Events::CEventDispatcher* pDispatcher, const Events::CEventBase& Event);
	bool				OnOSWindowSizeChanged(Events::CEventDispatcher* pDispatcher, const Events::CEventBase& Event);
	bool				OnOSWindowPaint(Events::CEventDispatcher* pDispatcher, const Events::CEventBase& Event);
	bool				OnOSWindowClosing(Events::CEventDispatcher* pDispatcher, const Events::CEventBase& Event);

	bool				Reset(D3DPRESENT_PARAMETERS& D3DPresentParams);
	void				Release();

	void				FillD3DPresentParams(const CRenderTargetDesc& BackBufferDesc, const CSwapChainDesc& SwapChainDesc, const Sys::COSWindow* pWindow, D3DPRESENT_PARAMETERS& D3DPresentParams) const;
	bool				GetCurrD3DPresentParams(const CD3D9SwapChain& SC, D3DPRESENT_PARAMETERS& D3DPresentParams) const;
	static D3DDEVTYPE	GetD3DDriverType(EGPUDriverType DriverType);
	bool				CreateD3DDevice(DWORD CurrAdapterID, EGPUDriverType CurrDriverType, D3DPRESENT_PARAMETERS D3DPresentParams);

	friend class CD3D9DriverFactory;

public:

	virtual ~CD3D9GPUDriver() {}

	virtual bool				Init(DWORD AdapterNumber, EGPUDriverType DriverType);
	virtual bool				CheckCaps(ECaps Cap);

	virtual DWORD				CreateSwapChain(const CRenderTargetDesc& BackBufferDesc, const CSwapChainDesc& SwapChainDesc, Sys::COSWindow* pWindow);
	virtual bool				DestroySwapChain(DWORD SwapChainID);
	virtual bool				SwapChainExists(DWORD SwapChainID) const;
	virtual bool				ResizeSwapChain(DWORD SwapChainID, unsigned int Width, unsigned int Height);
	virtual bool				SwitchToFullscreen(DWORD SwapChainID, const CDisplayDriver* pDisplay = NULL, const CDisplayMode* pMode = NULL);
	virtual bool				SwitchToWindowed(DWORD SwapChainID, const Data::CRect* pWindowRect = NULL);
	virtual bool				IsFullscreen(DWORD SwapChainID) const;
	virtual PRenderTarget		GetSwapChainRenderTarget(DWORD SwapChainID) const;
	//!!!get info, change info (or only recreate?)
	virtual bool				Present(DWORD SwapChainID);
	//virtual void				SaveScreenshot(DWORD SwapChainID, EImageFormat ImageFormat /*use image codec ref?*/, IO::CStream& OutStream);

	virtual PVertexLayout		CreateVertexLayout() { return NULL; } // Prefer GetVertexLayout() when possible
	virtual PVertexBuffer		CreateVertexBuffer() { return NULL; }
	virtual PIndexBuffer		CreateIndexBuffer() { return NULL; }
	virtual PRenderTarget		CreateRenderTarget(const CRenderTargetDesc& Desc);
	virtual PDepthStencilBuffer	CreateDepthStencilBuffer(const CRenderTargetDesc& Desc);

	//void						SetWireframe(bool Wire);
	//bool						IsWireframe() const { return Wireframe; }

	bool						GetD3DMSAAParams(EMSAAQuality MSAA, D3DFORMAT Format, D3DMULTISAMPLE_TYPE& OutType, DWORD& OutQuality) const;
	IDirect3DDevice9*			GetD3DDevice() const { return pD3DDevice; }
};

typedef Ptr<CD3D9GPUDriver> PD3D9GPUDriver;

}

#endif
