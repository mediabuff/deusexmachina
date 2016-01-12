#pragma once
#ifndef __DEM_L1_UI_CONTEXT_H__
#define __DEM_L1_UI_CONTEXT_H__

#include <UI/UIWindow.h>
#include <Events/EventsFwd.h>

// UI context is a set of elements rendered to one target and receiving the same input.
// Each engine window or viewport may have it's own UI context independent of each other.

namespace UI
{
typedef Ptr<class CUIWindow> PUIWindow;

class CUIContext: public Core::CObject
{
private:

	CUIWindow*							pRootWindow; //???smart ptr?
	CEGUI::GUIContext*					pCtx;
	//!!!OS window! at least for input

	DECLARE_EVENT_HANDLER(OSInput, OnOSWindowInput);
	DECLARE_EVENT_HANDLER(KeyDown, OnKeyDown);
	DECLARE_EVENT_HANDLER(KeyUp, OnKeyUp);
	DECLARE_EVENT_HANDLER(CharInput, OnCharInput);
	DECLARE_EVENT_HANDLER(MouseMove, OnMouseMove);
	DECLARE_EVENT_HANDLER(MouseBtnDown, OnMouseBtnDown);
	DECLARE_EVENT_HANDLER(MouseBtnUp, OnMouseBtnUp);
	DECLARE_EVENT_HANDLER(MouseWheel, OnMouseWheel);

public:

	//!!!pass OS window and render target params, either intermediate RT or swap chain index! or unify?
	CUIContext(): pCtx(NULL), pRootWindow(NULL) {}
	~CUIContext();

	void				Init(CEGUI::GUIContext* pContext);
	//CEGUI::GUIContext*	GetCEGUIContext() const { return pCtx; }

	// Pass absolute viewport coordinates here
	bool				Render(float Left, float Top, float Right, float Bottom);

	bool				SubscribeOnInput(Events::CEventDispatcher* pDispatcher, U16 Priority);
	void				UnsubscribeFromInput();

	void				SetRootWindow(CUIWindow* pWindow);
	CUIWindow*			GetRootWindow() const { return pRootWindow; }
	void				ShowGUI();
	void				HideGUI();
	void				ShowMouseCursor();
	void				HideMouseCursor();
	void				SetDefaultMouseCursor(const char* pImageName); //???also add SetMouseCursor?
	bool				IsGUIVisible() const;
	bool				IsMouseCursorVisible() const;
	bool				IsMouseOverGUI() const;
};

typedef Ptr<CUIContext> PUIContext;

}

#endif
