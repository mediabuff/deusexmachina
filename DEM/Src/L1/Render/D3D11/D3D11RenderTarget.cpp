#include "D3D11RenderTarget.h"

//#include <Render/D3D11/D3D11DriverFactory.h>

#define WIN32_LEAN_AND_MEAN
#include <d3d11.h>

namespace Render
{

bool CD3D11RenderTarget::Create(ID3D11Texture2D* pTexture, ID3D11RenderTargetView* pRTV)
{
	n_assert(pTexture && pRTV);

	// initialize RTTexture with ID3D11Texture2D
	//???pass pD3DDevice here ad create RTView here??

	OK;
}
//---------------------------------------------------------------------

void CD3D11RenderTarget::Destroy()
{
}
//---------------------------------------------------------------------

}