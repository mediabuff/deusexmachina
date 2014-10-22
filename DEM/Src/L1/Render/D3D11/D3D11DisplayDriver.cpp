#include "D3D11DisplayDriver.h"

#include <Render/D3D11/D3D11DriverFactory.h>
#include <Core/Factory.h>
#define WIN32_LEAN_AND_MEAN
#include <DXGI.h>

namespace Render
{
__ImplementClass(Render::CD3D11DisplayDriver, 'D1DD', Render::CDisplayDriver);

bool CD3D11DisplayDriver::Init(DWORD AdapterNumber, DWORD OutputNumber)
{
	if (!CDisplayDriver::Init(AdapterNumber, OutputNumber)) FAIL;

	IDXGIAdapter1* pAdapter = NULL;
	if (!SUCCEEDED(D3D11DrvFactory->GetDXGIFactory()->EnumAdapters1(Adapter, &pAdapter)))
	{
		Term();
		FAIL;
	}

	if (!SUCCEEDED(pAdapter->EnumOutputs(Output, &pDXGIOutput)))
	{
		pAdapter->Release();
		Term();
		FAIL;
	}

	pAdapter->Release();
	OK;
}
//---------------------------------------------------------------------

void CD3D11DisplayDriver::InternalTerm()
{
	SAFE_RELEASE(pDXGIOutput);
}
//---------------------------------------------------------------------

DWORD CD3D11DisplayDriver::GetAvailableDisplayModes(EPixelFormat Format, CArray<CDisplayMode>& OutModes) const
{
	if (!pDXGIOutput) return 0;

	DXGI_FORMAT DXGIFormat = CD3D11DriverFactory::PixelFormatToDXGIFormat(Format);

	HRESULT hr;
	UINT ModeCount = 0;
	DXGI_MODE_DESC* pDXGIModes = NULL;
	do
	{
		if (pDXGIModes) _freea(pDXGIModes);
		if (!SUCCEEDED(pDXGIOutput->GetDisplayModeList(DXGIFormat, 0, &ModeCount, NULL)) || !ModeCount) return 0;
		pDXGIModes = (DXGI_MODE_DESC*)_malloca(sizeof(DXGI_MODE_DESC) * ModeCount);
		hr = pDXGIOutput->GetDisplayModeList(DXGIFormat, 0, &ModeCount, pDXGIModes);
	}
	while (hr == DXGI_ERROR_MORE_DATA); // Somtimes new modes become available right between two calls, see DXGI docs

	// This code doesn't check for possible duplication with modes already in array
	CDisplayMode* pNewMode = OutModes.Reserve(ModeCount);
	for (UINT i = 0; i < ModeCount; ++i)
	{
		DXGI_MODE_DESC& DXGIMode = pDXGIModes[i];
		pNewMode->Width = DXGIMode.Width;
		pNewMode->Height = DXGIMode.Height;
		pNewMode->PixelFormat = CD3D11DriverFactory::DXGIFormatToPixelFormat(DXGIMode.Format);
		pNewMode->RefreshRate.Numerator = DXGIMode.RefreshRate.Numerator;
		pNewMode->RefreshRate.Denominator = DXGIMode.RefreshRate.Denominator;
		pNewMode->Stereo = false; // DXGI 1.2 and above only
	}

	_freea(pDXGIModes);

	return ModeCount;
}
//---------------------------------------------------------------------

bool CD3D11DisplayDriver::SupportsDisplayMode(const CDisplayMode& Mode) const
{
	if (!pDXGIOutput) FAIL;

	DXGI_FORMAT DXGIFormat = CD3D11DriverFactory::PixelFormatToDXGIFormat(Mode.PixelFormat);

	HRESULT hr;
	UINT ModeCount = 0;
	DXGI_MODE_DESC* pDXGIModes = NULL;
	do
	{
		if (pDXGIModes) _freea(pDXGIModes);
		if (!SUCCEEDED(pDXGIOutput->GetDisplayModeList(DXGIFormat, 0, &ModeCount, NULL)) || !ModeCount) FAIL;
		pDXGIModes = (DXGI_MODE_DESC*)_malloca(sizeof(DXGI_MODE_DESC) * ModeCount);
		hr = pDXGIOutput->GetDisplayModeList(DXGIFormat, 0, &ModeCount, pDXGIModes);
	}
	while (hr == DXGI_ERROR_MORE_DATA); // Sometimes new modes become available right between two calls, see DXGI docs

	for (UINT i = 0; i < ModeCount; ++i)
	{
		DXGI_MODE_DESC& DXGIMode = pDXGIModes[i];
		if (Mode.Width == DXGIMode.Width &&
			Mode.Height == DXGIMode.Height &&
			DXGIFormat == DXGIMode.Format && //???doesn't always match?
			Mode.RefreshRate.Numerator == DXGIMode.RefreshRate.Numerator &&
			Mode.RefreshRate.Denominator == DXGIMode.RefreshRate.Denominator &&
			!Mode.Stereo)
		{
			_freea(pDXGIModes);
			OK;
		}
	}

	_freea(pDXGIModes);
	FAIL;
}
//---------------------------------------------------------------------

bool CD3D11DisplayDriver::GetCurrentDisplayMode(CDisplayMode& OutMode) const
{
	if (!pDXGIOutput) FAIL;

	//???or adapter device name?

	DXGI_OUTPUT_DESC Desc;
	if (!SUCCEEDED(pDXGIOutput->GetDesc(&Desc))) FAIL;

	MONITORINFOEX Win32MonitorInfo;
	::ZeroMemory(&Win32MonitorInfo, sizeof(Win32MonitorInfo));
	Win32MonitorInfo.cbSize = sizeof(Win32MonitorInfo);
	if (!::GetMonitorInfo(Desc.Monitor, &Win32MonitorInfo)) FAIL;

	// Can't believe, but there is no way to obtain current display mode via DXGI without a swap chain
	DEVMODE DevMode = { 0 };
	if (!::EnumDisplaySettings(Win32MonitorInfo.szDevice, ENUM_CURRENT_SETTINGS, &DevMode)) FAIL;

	//???enumerate all matching modes and select one?
	OutMode.Width = DevMode.dmPelsWidth;
	OutMode.Height = DevMode.dmPelsHeight;
	OutMode.RefreshRate.Numerator = DevMode.dmDisplayFrequency;
	OutMode.RefreshRate.Denominator = 1;
	//!!!OutMode.PixelFormat = 
	OutMode.Stereo = false; //???how to be when it could be stereo?

	OK;
}
//---------------------------------------------------------------------

bool CD3D11DisplayDriver::GetDisplayMonitorInfo(CMonitorInfo& OutInfo) const
{
	if (!pDXGIOutput) FAIL;

	DXGI_OUTPUT_DESC Desc;
	if (!SUCCEEDED(pDXGIOutput->GetDesc(&Desc))) FAIL;

	MONITORINFO Win32MonitorInfo = { sizeof(Win32MonitorInfo), 0 };
	if (!::GetMonitorInfo(Desc.Monitor, &Win32MonitorInfo)) FAIL;

	OutInfo.Left = (ushort)Win32MonitorInfo.rcMonitor.left;
	OutInfo.Top = (ushort)Win32MonitorInfo.rcMonitor.top;
	OutInfo.Width = (ushort)(Win32MonitorInfo.rcMonitor.right - Win32MonitorInfo.rcMonitor.left);
	OutInfo.Height = (ushort)(Win32MonitorInfo.rcMonitor.bottom - Win32MonitorInfo.rcMonitor.top);
	OutInfo.IsPrimary = Win32MonitorInfo.dwFlags & MONITORINFOF_PRIMARY;
	//!!!device name can be obtained from adapter or MONITORINFOEX!

	OK;
}
//---------------------------------------------------------------------

}