#pragma once
#ifndef __DEM_L1_FRAME_RENDER_PHASE_H__
#define __DEM_L1_FRAME_RENDER_PHASE_H__

#include <Core/Object.h>
#include <Data/FixedArray.h>
#include <Data/StringID.h>
#include <Data/Flags.h>

// Frame rendering phase implements a single algorithm in a render path.
// Algorithm may implement rendering into RT/MRT/DSV, advanced culling
// technique or whatever user wants.

namespace Data
{
	class CParams;
}

namespace Frame
{
class CView;

class CRenderPhase: public Core::CObject //???need? lives only in RP!
{
	__DeclareClassNoFactory;

protected:

	CStrID Name;

public:

	//CFixedArray<PRenderTarget>	RenderTargets;
	//PDepthStencilBuffer			DepthStencil;

	//Data::CFlags				ClearFlags;
	//CFixedArray<U32>			RTClearColors;		// 32-bit ARGB each
	//float						DepthClearValue;	// 0.f .. 1.f
	//U8						StencilClearValue;

	//CRenderPhase(): ClearFlags(0), DepthClearValue(1.f), StencilClearValue(0) {}

	virtual ~CRenderPhase() {}

	virtual bool Init(CStrID PhaseName, const Data::CParams& Desc) { Name = PhaseName; OK; }
	virtual bool Render(CView& View) = 0;
};

typedef Ptr<CRenderPhase> PRenderPhase;

}

#endif
