#pragma once
#ifndef __DEM_L1_SHADER_LOADER_H__
#define __DEM_L1_SHADER_LOADER_H__

#include <Resources/ResourceCreator.h>

// Shader loaders are intended to load GPU shader objects. It is a class of
// different loaders each suited for a different shader type and 3D API.

namespace Render
{
	typedef Ptr<class CGPUDriver> PGPUDriver;
	typedef Ptr<class CShaderLibrary> PShaderLibrary;
}

namespace Resources
{

class CShaderLoader: public IResourceCreator
{
public:

	Render::PGPUDriver		GPU;
	Render::PShaderLibrary	ShaderLibrary; //!!!only for input signatures in D3D11 vertex shaders! redesign?
};

typedef Ptr<CShaderLoader> PShaderLoader;

}

#endif
