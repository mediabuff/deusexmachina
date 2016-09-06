#include "ParamsUtils.h"

#include <Data/HRDParser.h>
#include <Data/Buffer.h>
#include <IO/IOServer.h>
#include <IO/HRDWriter.h>
#include <IO/BinaryReader.h>
#include <IO/BinaryWriter.h>

namespace ParamsUtils
{

bool LoadParamsFromHRD(const char* pFileName, Data::PParams& OutParams)
{
	Data::CBuffer Buffer;
	if (!IOSrv->LoadFileToBuffer(pFileName, Buffer)) return NULL;

	Data::PParams Params;
	Data::CHRDParser Parser;
	return Parser.ParseBuffer((const char*)Buffer.GetPtr(), Buffer.GetSize(), OutParams);
}
//---------------------------------------------------------------------

bool LoadParamsFromPRM(const char* pFileName, Data::PParams& OutParams)
{
	IO::PStream File = IOSrv->CreateStream(pFileName);
	if (!File->Open(IO::SAM_READ)) return NULL;
	IO::CBinaryReader Reader(*File);

	Data::PParams Params = n_new(Data::CParams);
	if (!Reader.ReadParams(*Params)) FAIL;
	OutParams = Params;
	OK;
}
//---------------------------------------------------------------------

bool LoadDescFromPRM(const char* pRootPath, const char* pRelativeFileName, Data::PParams& OutParams)
{
	Data::PParams Main;
	if (!ParamsUtils::LoadParamsFromPRM(CString(pRootPath) + pRelativeFileName, Main)) FAIL;
	if (Main.IsNullPtr()) FAIL;

	CString BaseName;
	if (Main->Get(BaseName, CStrID("_Base_")))
	{
		n_assert(BaseName != pRelativeFileName);
		if (!LoadDescFromPRM(pRootPath, BaseName, OutParams)) FAIL;
		OutParams->Merge(*Main, Data::Merge_AddNew | Data::Merge_Replace | Data::Merge_Deep); //!!!can specify merge flags in Desc!
	}
	else OutParams = n_new(Data::CParams(*Main));

	OK;
}
//---------------------------------------------------------------------

bool SaveParamsToHRD(const char* pFileName, const Data::CParams& Params)
{
	IO::PStream File = IOSrv->CreateStream(pFileName);
	if (!File->Open(IO::SAM_WRITE)) FAIL;
	IO::CHRDWriter Writer(*File);
	return Writer.WriteParams(Params);
}
//---------------------------------------------------------------------

bool SaveParamsToPRM(const char* pFileName, const Data::CParams& Params)
{
	IO::PStream File = IOSrv->CreateStream(pFileName);
	if (!File->Open(IO::SAM_WRITE)) FAIL;
	IO::CBinaryWriter Writer(*File);
	return Writer.WriteParams(Params);
}
//---------------------------------------------------------------------

}