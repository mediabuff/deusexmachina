#pragma once
#ifndef __DEM_TOOLS_D3D9_SHADER_REFLECTION_H__
#define __DEM_TOOLS_D3D9_SHADER_REFLECTION_H__

#include <Data/String.h>
#include <Data/Dictionary.h>

// Code is obtained from
// http://www.gamedev.net/topic/648016-replacement-for-id3dxconstanttable/
// This version has some cosmetic changes

// Don't change values, they are saved to file
enum ERegisterSet
{
	RS_BOOL,
	RS_INT4,
	RS_FLOAT4,
	RS_SAMPLER
};

struct CD3D9ConstantDesc
{
	CString			Name;
	ERegisterSet	RegisterSet;
	UPTR			RegisterIndex;
	UPTR			RegisterCount;
	UPTR			Rows;
	UPTR			Columns;
	UPTR			Elements;
	UPTR			StructMembers;
	UPTR			Bytes;
};

bool D3D9Reflect(const void* pData, UPTR Size, CArray<CD3D9ConstantDesc>& OutConsts, CString& OutCreator);
void D3D9FindSamplerTextures(const char* pSrcText, UPTR Size, CDict<CString, CString>& OutSampToTex);

#endif
