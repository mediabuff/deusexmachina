#pragma once
#ifndef __DEM_L2_GAME_LEVEL_VIEW_H__
#define __DEM_L2_GAME_LEVEL_VIEW_H__

#include <Frame/View.h>
#include <Data/StringID.h>
#include <Math/Vector3.h>

// Represents a client view of a game location. Manages rendering and input.
// In MVC pattern level view would be a View.

namespace Game
{
typedef Ptr<class CGameLevel> PGameLevel;

class CGameLevelView
{
protected:

	HHandle				Handle;
	PGameLevel			Level;
	Frame::CView		View;
	//???store game camera here or obtain from manager? load camera in view loading!

	CArray<CStrID>		SelectedEntities;

	//???!!!per-view?! or assume there is only one mouse? may store struct CView / Handle + isect info + level ID if required
	CStrID				EntityUnderMouse;
	bool				HasMouseIsect;
	vector3				MousePos3D;

public:

	bool					Setup(CGameLevel& GameLevel, HHandle hView);

	HHandle					GetHandle() const { return Handle; }
	CGameLevel*				GetLevel() const { return Level.GetUnsafe(); }
	Frame::CView&			GetFrameView() { return View; }
	const vector3&			GetCenterOfInterest() const;

	void					AddToSelection(CStrID EntityID);
	bool					RemoveFromSelection(CStrID EntityID);
	void					ClearSelection() { SelectedEntities.Clear(); }
	const CArray<CStrID>&	GetSelection() const { return SelectedEntities; }
	UPTR					GetSelectedCount() const { return SelectedEntities.GetCount(); }
	bool					IsSelected(CStrID EntityID) const { return SelectedEntities.Contains(EntityID); }

	//???void UpdateMouseIntersectionInfo(); //???all at once or user request for each view?
	bool					HasMouseIntersection(/*HHandle hView*/) const { return HasMouseIsect; }
	CStrID					GetEntityUnderMouseUID(/*HHandle hView*/) const { return EntityUnderMouse; }
	const vector3&			GetMousePos3D(/*HHandle hView*/) const { return MousePos3D; }
};

}

#endif