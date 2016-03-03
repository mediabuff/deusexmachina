#pragma once
#ifndef __DEM_TOOLS_SHADER_COMPILER_H__
#define __DEM_TOOLS_SHADER_COMPILER_H__

#include <stdint.h>

typedef uint32_t U32;	// Unsigned 32-bit integer
struct CSM30ShaderMeta;
struct CD3D11ShaderMeta;

#define DEM_DLL_EXPORT	__declspec(dllexport)
#define DEM_DLLCALL		__cdecl

enum EShaderType
{
	ShaderType_Vertex = 0,	// Don't change order and starting index
	ShaderType_Pixel,
	ShaderType_Geometry,
	ShaderType_Hull,
	ShaderType_Domain,

	ShaderType_COUNT
};

#ifdef __cplusplus
extern "C"
{
#endif

// For static loading
DEM_DLL_EXPORT bool DEM_DLLCALL OpenShaderDatabase(const char* pDBFilePath);
DEM_DLL_EXPORT bool DEM_DLLCALL CompileShader(const char* pSrcPath, EShaderType ShaderType, U32 Target, const char* pEntryPoint,
											  const char* pDefines, bool Debug, U32& ObjectFileID, U32& InputSignatureFileID);
DEM_DLL_EXPORT bool DEM_DLLCALL LoadShaderMetadataByObjectFileID(U32 ID, U32& OutTarget, CSM30ShaderMeta& OutD3D9Meta, CD3D11ShaderMeta& OutD3D11Meta);

#ifdef __cplusplus
}
#endif

// For dynamic loading
typedef bool (DEM_DLLCALL *FDEMShaderCompiler_OpenShaderDatabase)(const char* pDBFilePath);
typedef bool (DEM_DLLCALL *FDEMShaderCompiler_CompileShader)(const char* pSrcPath, EShaderType ShaderType, U32 Target, const char* pEntryPoint,
															 const char* pDefines, bool Debug, U32& ObjectFileID, U32& InputSignatureFileID);
typedef bool (DEM_DLLCALL *FDEMShaderCompiler_LoadShaderMetadataByObjectFileID)(U32 ID, U32& OutTarget, CSM30ShaderMeta& OutD3D9Meta, CD3D11ShaderMeta& OutD3D11Meta);

#endif
