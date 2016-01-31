#pragma once
#ifndef __DEM_L1_RENDER_EFFECT_TECH_H__
#define __DEM_L1_RENDER_EFFECT_TECH_H__

#include <Data/Array.h>

// A particular implementation of an effect for a specific input data. It includes
// geometry specifics (static/skinned), environment (light count, fog), additional
// material effects (parallax, alpha test). A combination of input states is
// represented as a set of feature flags. Technique can accept different combinations
// of these flags, though ones close to each other. So, instead of storing each
// particular combination, we specify what flags should be set and what should be
// unset, and allow other flags to have any state.

namespace Render
{
typedef Data::CFlags CFeatureFlags;
typedef Ptr<class CRenderState> PRenderState;

class CEffectTech
{
public:

	CFeatureFlags			IncludedFFlags;		// Feature flags that must be set to use this tech
	CFeatureFlags			ExcludedFFlags;		// Feature flags that must be unset to use this tech

	CArray<PRenderState>	Passes;

	//PShaderParamsDesc		ParamsDesc;
	//CArray<PConstantBuffer>	ConstBuffers;		// References to constant buffers used by this tech, not used buffers are NULL

	bool					IsMatching(CFeatureFlags FFlags) const { return FFlags.Is(IncludedFFlags) && FFlags.IsNot(ExcludedFFlags); }

//	//SetConst
//	//SetResource
//	//SetSampler
//	//ApplyParams
//	virtual void			SetBool(HShaderParam Handle, const bool* pValues, UPTR Count) = 0;
//	virtual void			SetIntAsBool(HShaderParam Handle, const int* pValues, UPTR Count) = 0;
//	virtual void			SetInt(HShaderParam Handle, const int* pValues, UPTR Count) = 0;
//	virtual void			SetFloat(HShaderParam Handle, const float* pValues, UPTR Count) = 0;
//	virtual void			SetVector4(HShaderParam Handle, const vector4* pValues, UPTR Count) = 0;
//	virtual void			SetMatrix44(HShaderParam Handle, const matrix44* pValues, UPTR Count) = 0;
//	//???need? virtual void			SetMatrix44Transpose(HShaderParam Handle, const matrix44* pValues, UPTR Count) = 0;
////	virtual void			SetTexture(HShaderParam Handle, CTexture* pTexture) = 0;
//	//???virtual void			SetBuffer(HShaderParam Handle, CTexture* pTexture) = 0;
////	virtual void			SetSamplerState(HShaderParam Handle, CSamplerState* pSamplerState) = 0;
};

}

#endif
