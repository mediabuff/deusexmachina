#pragma once
#ifndef __DEM_L1_RENDER_D3D11_DRIVER_FACTORY_H__
#define __DEM_L1_RENDER_D3D11_DRIVER_FACTORY_H__

#include <Render/VideoDriverFactory.h>
#include <Data/Singleton.h>

// Direct3D 11 and DXGI 1.1 implementation of CVideoDriverFactory

//???rename to DXGI[version]DriverFactory?

struct IDXGIFactory1;
enum DXGI_FORMAT;

namespace Render
{
#define D3D11DrvFactory CD3D11DriverFactory::Instance()

class CD3D11DriverFactory: public CVideoDriverFactory
{
	__DeclareClassNoFactory;
	__DeclareSingleton(CD3D11DriverFactory);

protected:

	IDXGIFactory1*	pDXGIFactory;
	DWORD			AdapterCount;	// Valid during a lifetime of the DXGI factory object

public:

	CD3D11DriverFactory(): pDXGIFactory(NULL), AdapterCount(0) { __ConstructSingleton; }
	virtual ~CD3D11DriverFactory() { if (IsOpened()) Close(); __DestructSingleton; }

	static DXGI_FORMAT		PixelFormatToDXGIFormat(EPixelFormat Format);
	static EPixelFormat		DXGIFormatToPixelFormat(DXGI_FORMAT D3DFormat);

	bool					Open();
	void					Close();
	bool					IsOpened() const { return !!pDXGIFactory; }

	virtual bool			AdapterExists(DWORD Adapter) const { return Adapter < AdapterCount; }
	virtual DWORD			GetAdapterCount() const { return AdapterCount; }
	virtual bool			GetAdapterInfo(DWORD Adapter, CAdapterInfo& OutInfo) const;
	virtual DWORD			GetAdapterOutputCount(DWORD Adapter) const;
	virtual PDisplayDriver	CreateDisplayDriver(DWORD Adapter = 0, DWORD Output = 0);
	virtual PGPUDriver		CreateGPUDriver(DWORD Adapter = Adapter_AutoSelect, EGPUDriverType DriverType = GPU_AutoSelect);

	IDXGIFactory1*			GetDXGIFactory() const { return pDXGIFactory; }
};

}

#endif