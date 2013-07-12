#pragma once
#ifndef __DEM_L1_ANIM_H__
#define __DEM_L1_ANIM_H__

#include <StdDEM.h>
#include <Scene/SceneFwd.h>
#include <mathlib/vector.h>

// Animation system constants and forward declarations

template<class TKey, class TVal> class CDict;

namespace Anim
{

/*
enum ELoopType
{
	LoopType_Clamp,
	LoopType_Loop
};
*/

class CAnimTrack
{
public:

	vector4			ConstValue;
	Scene::EChannel	Channel; //!!!???can avoid storing it? needed only at Setup() time, move to loader?
};

struct CSampler
{
	CAnimTrack* pTrackT;
	CAnimTrack* pTrackR;
	CAnimTrack* pTrackS;

	CSampler(): pTrackT(NULL), pTrackR(NULL), pTrackS(NULL) {}
};

}

#endif