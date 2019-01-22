#include "CDLODDataLoader.h"

#include <Render/CDLODData.h>
#include <Resources/ResourceManager.h>
#include <IO/BinaryReader.h>

namespace Resources
{

const Core::CRTTI& CCDLODDataLoader::GetResultType() const
{
	return Render::CCDLODData::RTTI;
}
//---------------------------------------------------------------------

PResourceObject CCDLODDataLoader::CreateResource(CStrID UID)
{
	if (!pResMgr) return nullptr;

	const char* pOutSubId;
	IO::PStream Stream = pResMgr->CreateResourceStream(UID, pOutSubId);
	if (!Stream || !Stream->Open(IO::SAM_READ, IO::SAP_SEQUENTIAL)) return nullptr;

	IO::CBinaryReader Reader(*Stream);

	U32 Magic;
	if (!Reader.Read<U32>(Magic) || Magic != 'CDLD') return nullptr;

	U32 Version;
	if (!Reader.Read<U32>(Version)) return nullptr;

	Render::PCDLODData Obj = n_new(Render::CCDLODData);

	if (!Reader.Read(Obj->HFWidth)) return nullptr;
	if (!Reader.Read(Obj->HFHeight)) return nullptr;
	if (!Reader.Read(Obj->PatchSize)) return nullptr;
	if (!Reader.Read(Obj->LODCount)) return nullptr;
	const U32 MinMaxDataSize = Reader.Read<U32>();
	if (!Reader.Read(Obj->VerticalScale)) return nullptr;
	if (!Reader.Read(Obj->Box.Min.x)) return nullptr;
	if (!Reader.Read(Obj->Box.Min.y)) return nullptr;
	if (!Reader.Read(Obj->Box.Min.z)) return nullptr;
	if (!Reader.Read(Obj->Box.Max.x)) return nullptr;
	if (!Reader.Read(Obj->Box.Max.y)) return nullptr;
	if (!Reader.Read(Obj->Box.Max.z)) return nullptr;

	// Skip height map, it is loaded as texture in a separate loader
	const UPTR HeightMapDataSize = Obj->HFWidth * Obj->HFHeight * sizeof(unsigned short);
	if (!Stream->Seek(HeightMapDataSize, IO::Seek_Current)) return nullptr;

	Obj->pMinMaxData = (I16*)n_malloc(MinMaxDataSize);
	Reader.GetStream().Read(Obj->pMinMaxData, MinMaxDataSize);

	UPTR PatchesW = (Obj->HFWidth - 1 + Obj->PatchSize - 1) / Obj->PatchSize;
	UPTR PatchesH = (Obj->HFHeight - 1 + Obj->PatchSize - 1) / Obj->PatchSize;
	UPTR Offset = 0;
	Obj->MinMaxMaps.SetSize(Obj->LODCount);
	for (UPTR LOD = 0; LOD < Obj->LODCount; ++LOD)
	{
		Render::CCDLODData::CMinMaxMap& MMMap = Obj->MinMaxMaps[LOD];
		MMMap.PatchesW = PatchesW;
		MMMap.PatchesH = PatchesH;
		MMMap.pData = Obj->pMinMaxData + Offset;
		Offset += PatchesW * PatchesH * 2;
		PatchesW = (PatchesW + 1) / 2;
		PatchesH = (PatchesH + 1) / 2;
	}

	UPTR TopPatchSize = Obj->PatchSize << (Obj->LODCount - 1);
	Obj->TopPatchCountW = (Obj->HFWidth - 1 + TopPatchSize - 1) / TopPatchSize;
	Obj->TopPatchCountH = (Obj->HFHeight - 1 + TopPatchSize - 1) / TopPatchSize;

	return Obj.Get();
}
//---------------------------------------------------------------------

}