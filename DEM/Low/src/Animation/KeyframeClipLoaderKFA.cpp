#include "KeyframeClipLoaderKFA.h"

#include <Animation/KeyframeClip.h>
#include <Resources/ResourceManager.h>
#include <IO/BinaryReader.h>

namespace Resources
{

const Core::CRTTI& CKeyframeClipLoaderKFA::GetResultType() const
{
	return Anim::CKeyframeClip::RTTI;
}
//---------------------------------------------------------------------

PResourceObject CKeyframeClipLoaderKFA::CreateResource(CStrID UID)
{
	if (!pResMgr) return nullptr;

	const char* pOutSubId;
	IO::PStream Stream = pResMgr->CreateResourceStream(UID, pOutSubId);
	if (!Stream || !Stream->Open(IO::SAM_READ, IO::SAP_SEQUENTIAL)) return nullptr;

	IO::CBinaryReader Reader(*Stream);

	U32 Magic;
	if (!Reader.Read(Magic) || Magic != 'KFAN') return nullptr;

	float Duration;
	if (!Reader.Read(Duration)) return nullptr;

	U32 TrackCount;
	if (!Reader.Read(TrackCount)) return nullptr;

	CArray<Anim::CKeyframeTrack> Tracks;
	CArray<CStrID> TrackMapping;

	Anim::CKeyframeTrack* pTrack = Tracks.Reserve(TrackCount);
	CStrID* pMap = TrackMapping.Reserve(TrackCount);
	for (UPTR i = 0; i < TrackCount; ++i, ++pTrack, ++pMap)
	{
		Reader.Read(*pMap);

		pTrack->Channel = (Scene::ETransformChannel)Reader.Read<int>();

		U32 KeyCount;
		if (!Reader.Read(KeyCount)) return nullptr;

		if (!KeyCount) Reader.Read(pTrack->ConstValue);
		else
		{
			pTrack->Keys.SetSize(KeyCount);
			Stream->Read(pTrack->Keys.GetPtr(), KeyCount * sizeof(Anim::CKeyframeTrack::CKey));
		}
	}

	//!!!load event tracks!
	CArray<Anim::CEventTrack> EventTracks;

	//!!!DBG TMP!
	Anim::CEventTrack& ET = *EventTracks.Reserve(1);
	ET.Keys.SetSize(3);
	ET.Keys[0].Time = 0.f;
	ET.Keys[0].EventID = CStrID("OnAnimStart");
	ET.Keys[1].Time = Duration * 0.5f;
	ET.Keys[1].EventID = CStrID("OnAnimHalf");
	ET.Keys[2].Time = Duration;
	ET.Keys[2].EventID = CStrID("OnAnimEnd");

	ET.Keys.Sort();

	//???load directly to Clip fields?
	Anim::PKeyframeClip Clip = n_new(Anim::CKeyframeClip);
	Clip->Setup(Tracks, TrackMapping, &EventTracks, Duration);

	return Clip.Get();
}
//---------------------------------------------------------------------

}
