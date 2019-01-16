#include "Application.h"

#include <IO/IOServer.h>
#include <IO/FSBrowser.h>
#include <Events/EventServer.h>
#include <Events/Subscription.h>
#include <Math/Math.h>
#include <System/Platform.h>
#include <System/OSWindow.h>
#include <Render/SwapChain.h>
#include <Render/GPUDriver.h>
#include <Data/ParamsUtils.h>

namespace DEM { namespace Core
{

//???empty constructor, add Init to process init-time failures?
CApplication::CApplication(Sys::IPlatform& _Platform)
	: Platform(_Platform)
{
	// check multiple instances

	//???move RNG instance to an application instead of static vars? pass platform system time as seed?
	// RNG is initialized in constructor to be available anywhere
	Math::InitRandomNumberGenerator();

	// create default file system from platform
	// setup hard assigns from platform and application

	n_new(Events::CEventServer);
	IOServer.reset(n_new(IO::CIOServer));
}
//---------------------------------------------------------------------

CApplication::~CApplication()
{
	if (Events::CEventServer::HasInstance()) n_delete(EventSrv);
}
//---------------------------------------------------------------------

IO::CIOServer& CApplication::IO() const
{
	return *IOServer;
}
//---------------------------------------------------------------------

CStrID CApplication::CreateUserProfile(const char* pUserID)
{
	CString UserStr(pUserID);
	if (UserStr.IsEmpty() || UserStr.ContainsAny("\t\n\r\\/:?&%$#@!~")) return CStrID::Empty;

	CString Path = "AppData:profiles/" + UserStr;
	if (IO().DirectoryExists(Path)) return CStrID::Empty;

	if (!IO().CreateDirectory(Path)) return CStrID::Empty;
	if (!IO().CreateDirectory(Path + "/saves")) return CStrID::Empty;
	if (!IO().CreateDirectory(Path + "/screenshots")) return CStrID::Empty;
	if (!IO().CreateDirectory(Path + "/current")) return CStrID::Empty;

	//!!!DBG TMP!
	//!!!template must be packed into NPK or reside in bin/data or smth!
	if (!IO().CopyFile("../content/DefaultUserSettings.hrd", Path + "/Settings.hrd")) return CStrID::Empty;

	return CStrID(pUserID);
}
//---------------------------------------------------------------------

UPTR CApplication::EnumUserProfiles(CArray<CStrID>& Out) const
{
	CString ProfilesDir("AppData:profiles/");
	if (!IO().DirectoryExists(ProfilesDir)) return 0;

	const UPTR OldCount = Out.GetCount();

	IO::CFSBrowser Browser;
	Browser.SetAbsolutePath(ProfilesDir);
	if (!Browser.IsCurrDirEmpty()) do
	{
		if (Browser.IsCurrEntryDir())
			Out.Add(CStrID(Browser.GetCurrEntryName()));
	}
	while (Browser.NextCurrDirEntry());

	return Out.GetCount() - OldCount;
}
//---------------------------------------------------------------------

CStrID CApplication::ActivateUser(CStrID UserID)
{
	// if active, return ID
	// if no profile, return empty
	// - load user settings (or store inside a profile)
	// - register input translator
	// - connect all input devices to this user if it is the first user loaded (and now it is)
	return UserID;
}
//---------------------------------------------------------------------

void CApplication::ParseCommandLine(const char* pCmdLine)
{
	if (!pCmdLine || !*pCmdLine) return;

	//!!!DBG TMP!
	if (!strcmp(pCmdLine, "-O TestFloat=999.0"))
	{
		OverrideSettings = n_new(Data::CParams(1));
		OverrideSettings->Set<float>(CStrID("TestFloat"), 999.f);
	}
}

bool CApplication::LoadSettings(const char* pFilePath, bool Reload, CStrID UserID)
{
	Data::PParams Prm;
	if (!ParamsUtils::LoadParamsFromHRD(pFilePath, Prm)) FAIL;

	if (Reload || !GlobalSettings) GlobalSettings = Prm;
	else GlobalSettings->Merge(*Prm, Data::Merge_Replace | Data::Merge_Deep);

	OK;
}
//---------------------------------------------------------------------

int CApplication::GetIntSetting(const char* pKey, int Default, CStrID UserID)
{
	if (OverrideSettings)
	{
		Data::CData OverrideData;
		if (OverrideSettings->Get(OverrideData, CStrID(pKey)) && OverrideData.IsA<float>())
		{
			return OverrideData.GetValue<int>();
		}
	}

	if (UserID.IsValid())
	{
		// check user loaded
		// if not, assert and return default
		// if settings is set for user, return user's value
	}

	return GlobalSettings ? GlobalSettings->Get<int>(CStrID(pKey), Default) : Default;
}
//---------------------------------------------------------------------

float CApplication::GetFloatSetting(const char* pKey, float Default, CStrID UserID)
{
	if (OverrideSettings)
	{
		Data::CData OverrideData;
		if (OverrideSettings->Get(OverrideData, CStrID(pKey)) && OverrideData.IsA<float>())
		{
			return OverrideData.GetValue<float>();
		}
	}

	if (UserID.IsValid())
	{
		// check user loaded
		// if not, assert and return default
		// if settings is set for user, return user's value
	}

	return GlobalSettings ? GlobalSettings->Get<float>(CStrID(pKey), Default) : Default;
}
//---------------------------------------------------------------------

const CString& CApplication::GetStringSetting(const char* pKey, const CString& Default, CStrID UserID)
{
	if (OverrideSettings)
	{
		Data::CData OverrideData;
		if (OverrideSettings->Get(OverrideData, CStrID(pKey)) && OverrideData.IsA<float>())
		{
			return OverrideData.GetValue<CString>();
		}
	}

	if (UserID.IsValid())
	{
		// check user loaded
		// if not, assert and return default
		// if settings is set for user, return user's value
	}

	return GlobalSettings ? GlobalSettings->Get<CString>(CStrID(pKey), Default) : Default;
}
//---------------------------------------------------------------------

// Creates a GUI window most suitable for 3D scene rendering, based on app & profile settings
int CApplication::CreateRenderWindow(Render::CGPUDriver* pGPU, U32 Width, U32 Height)
{
	auto Wnd = Platform.CreateGUIWindow();
	Wnd->SetRect(Data::CRect(50, 50, Width, Height));

	Render::CRenderTargetDesc BBDesc;
	BBDesc.Format = Render::PixelFmt_DefaultBackBuffer;
	BBDesc.MSAAQuality = Render::MSAA_None;
	BBDesc.UseAsShaderInput = false;
	BBDesc.MipLevels = 0;
	BBDesc.Width = 0;
	BBDesc.Height = 0;

	Render::CSwapChainDesc SCDesc;
	SCDesc.BackBufferCount = 2;
	SCDesc.SwapMode = Render::SwapMode_CopyDiscard;
	SCDesc.Flags = Render::SwapChain_AutoAdjustSize | Render::SwapChain_VSync;

	int SwapChainID = pGPU->CreateSwapChain(BBDesc, SCDesc, Wnd);
	n_assert(pGPU->SwapChainExists(SwapChainID));
	return SwapChainID;
}
//---------------------------------------------------------------------

//???need Run()? use Init()?
bool CApplication::Run(/*initial state?*/)
{
	BaseTime = Platform.GetSystemTime();
	PrevTime = BaseTime;
	FrameTime = 0.0;

	// initialize systems

	// start FSM initial state (enter state)
	OK;
}
//---------------------------------------------------------------------

bool CApplication::Update()
{
	// Update time

	constexpr double MAX_FRAME_TIME = 0.25;

	const double CurrTime = Platform.GetSystemTime();
	FrameTime = CurrTime - PrevTime;
	if (FrameTime < 0.0) FrameTime = 1.0 / 60.0;
	else if (FrameTime > MAX_FRAME_TIME) FrameTime = MAX_FRAME_TIME;
	FrameTime *= TimeScale;

	PrevTime = CurrTime;

	// Process OS messages etc

	Platform.Update();

	// ...

	// Update current application state
	// ...

	//!!!DBG TMP!
	if (Exiting) FAIL;

	OK;
}
//---------------------------------------------------------------------

//!!!Init()'s pair!
void CApplication::Term()
{
}
//---------------------------------------------------------------------

void CApplication::ExitOnWindowClosed(Sys::COSWindow* pWindow)
{
	if (pWindow)
	{
		DISP_SUBSCRIBE_PEVENT(pWindow, OnClosing, CApplication, OnMainWindowClosing);
	}
	else
	{
		UNSUBSCRIBE_EVENT(OnClosing);
	}
}
//---------------------------------------------------------------------

bool CApplication::OnMainWindowClosing(Events::CEventDispatcher* pDispatcher, const Events::CEventBase& Event)
{
	UNSUBSCRIBE_EVENT(OnClosing);

	//FSM.RequestState(CStrID::Empty);

	//!!!DBG TMP!
	Exiting = true;

	OK;
}
//---------------------------------------------------------------------

}};
