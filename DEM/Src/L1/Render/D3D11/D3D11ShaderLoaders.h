#pragma once
#ifndef __DEM_L1_D3D11_SHADER_LOADERS_H__
#define __DEM_L1_D3D11_SHADER_LOADERS_H__

#include <Render/ShaderLoader.h>

// Direct3D11 shader loader implementations for different shader types

namespace Resources
{

class CD3D11VertexShaderLoader: public CShaderLoader
{
	__DeclareClass(CD3D11VertexShaderLoader);

public:

	virtual const Core::CRTTI&	GetResultType() const;
	virtual bool				IsProvidedDataValid() const;
	virtual bool				Load(CResource& Resource);
};

typedef Ptr<CD3D11VertexShaderLoader> PD3D11VertexShaderLoader;

class CD3D11PixelShaderLoader: public CShaderLoader
{
	__DeclareClass(CD3D11PixelShaderLoader);

public:

	virtual const Core::CRTTI&	GetResultType() const;
	virtual bool				IsProvidedDataValid() const;
	virtual bool				Load(CResource& Resource);
};

typedef Ptr<CD3D11PixelShaderLoader> PD3D11PixelShaderLoader;

}

#endif
