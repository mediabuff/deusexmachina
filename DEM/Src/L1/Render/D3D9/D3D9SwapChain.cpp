#include "D3D9SwapChain.h"

#include <Events/EventServer.h>

#define WIN32_LEAN_AND_MEAN
#define D3D_DISABLE_9EX
#include <d3d9.h>

namespace Render
{

void CD3D9SwapChain::Release()
{
	Sub_OnClosing = NULL;
	Sub_OnSizeChanged = NULL;
	Sub_OnToggleFullscreen = NULL;

	if (pSwapChain)
	{
		pSwapChain->Release();
		pSwapChain = NULL;
	}
	else
	{
		//???move this branch to a DestroySwapChain() method?
		Sys::Error("CD3D9SwapChain::Release() > Implicit swap chain destroyed! IMPLEMENT ME!!!");
		// Implicit swap chain
		//!!!destroy device!
		//???or deny and require user to call DestroyDevice (Term)?
	}
}
//---------------------------------------------------------------------

}