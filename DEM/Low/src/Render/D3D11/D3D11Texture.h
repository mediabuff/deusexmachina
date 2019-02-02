#pragma once
#include <Render/Texture.h>

// D3D11 implementation of VRAM texture

struct ID3D11Resource;
struct ID3D11Texture1D;
struct ID3D11Texture2D;
struct ID3D11Texture3D;
struct ID3D11ShaderResourceView;
enum D3D11_USAGE;

namespace Render
{

class CD3D11Texture: public CTexture
{
	__DeclareClass(CD3D11Texture);

protected:

	ID3D11Resource*				pD3DTex = nullptr; //???or union?
	ID3D11ShaderResourceView*	pSRView = nullptr;
	D3D11_USAGE					D3DUsage;

	void						InternalDestroy();

public:

	CD3D11Texture() {}
	virtual ~CD3D11Texture() { InternalDestroy(); }

	//???assert destroyed?
	bool						Create(PTextureData Data, ID3D11ShaderResourceView* pSRV); 
	bool						Create(PTextureData Data, ID3D11Resource* pTexture, ID3D11ShaderResourceView* pSRV);
	bool						Create(ID3D11Texture1D* pTexture, ID3D11ShaderResourceView* pSRV);
	bool						Create(ID3D11Texture2D* pTexture, ID3D11ShaderResourceView* pSRV);
	bool						Create(ID3D11Texture3D* pTexture, ID3D11ShaderResourceView* pSRV);
	virtual void				Destroy() { InternalDestroy(); }

	virtual bool				IsResourceValid() const { return !!pD3DTex; }

	ID3D11Resource*				GetD3DResource() const { return pD3DTex; }
	ID3D11Texture1D*			GetD3DTexture1D() const;
	ID3D11Texture2D*			GetD3DTexture2D() const;
	ID3D11Texture3D*			GetD3DTexture3D() const;
	ID3D11ShaderResourceView*	GetD3DSRView() const { return pSRView; }
	D3D11_USAGE					GetD3DUsage() const { return D3DUsage; }
};

typedef Ptr<CD3D11Texture> PD3D11Texture;

}
