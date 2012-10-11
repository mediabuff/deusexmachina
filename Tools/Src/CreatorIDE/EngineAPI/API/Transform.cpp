#include <StdAPI.h>
#include <Game/Mgr/FocusManager.h>
#include <Game/Mgr/EntityManager.h>
#include <Game/Mgr/EnvQueryManager.h>
#include <Loading/EntityFactory.h>
#include <App/CIDEApp.h>
#include <Physics/Event/SetTransform.h>
#include <Input/Prop/PropInput.h>
#include <Physics/Prop/PropAbstractPhysics.h>
#include <Physics/CharEntity.h>
#include <Physics/Composite.h>
#include <Prop/PropEditorTfmInput.h>

using namespace Properties;

static void PatchCurrentTfmEntity()
{
	if (CIDEApp->SelectedEntity.isvalid())
	{
		n_assert(CIDEApp->OldInputPropClass.IsEmpty());
		CIDEApp->SelectedEntity->Deactivate();
		Properties::CPropInput* pInput = CIDEApp->SelectedEntity->FindProperty<Properties::CPropInput>();
		if (pInput)
		{
			CIDEApp->OldInputPropClass = pInput->GetClassName();
			EntityFct->DetachProperty(*CIDEApp->SelectedEntity, CIDEApp->OldInputPropClass);
		}
		EntityFct->AttachProperty<Properties::CPropEditorTfmInput>(*CIDEApp->SelectedEntity);
		CIDEApp->SelectedEntity->Activate();
	}
	FocusMgr->SetInputFocusEntity(CIDEApp->SelectedEntity.get_unsafe());
}
//---------------------------------------------------------------------

static void RestoreCurrentTfmEntity()
{
	if (CIDEApp->SelectedEntity.isvalid())
	{
		CIDEApp->SelectedEntity->Deactivate();
		EntityFct->DetachProperty(*CIDEApp->SelectedEntity, "Properties::CPropInput");
		if (CIDEApp->OldInputPropClass.IsValid())
		{
			EntityFct->AttachProperty(*CIDEApp->SelectedEntity, CIDEApp->OldInputPropClass);
			CIDEApp->OldInputPropClass.Clear();
		}
		CIDEApp->SelectedEntity->Activate();
	}
}
//---------------------------------------------------------------------

API void Transform_SetEnabled(bool Enable)
{
	if (Enable == CIDEApp->TransformMode) return;

	CIDEApp->TransformMode = Enable;

	if (CIDEApp->TransformMode) PatchCurrentTfmEntity();
	else
	{
		RestoreCurrentTfmEntity();
		FocusMgr->SetInputFocusEntity(CIDEApp->EditorCamera);
	}
}
//---------------------------------------------------------------------

API void Transform_SetCurrentEntity(const char* UID)
{
	Game::PEntity Ent = EntityMgr->GetEntityByID(CStrID(UID));
	if (Ent.get_unsafe() == CIDEApp->SelectedEntity.get_unsafe()) return;
	if (CIDEApp->TransformMode) RestoreCurrentTfmEntity();
	CIDEApp->SelectedEntity = Ent;
	if (CIDEApp->TransformMode) PatchCurrentTfmEntity();
}
//---------------------------------------------------------------------

API void Transform_SetGroundRespectMode(bool Limit, bool Snap)
{
	CIDEApp->LimitToGround = Limit;
	CIDEApp->SnapToGround = Snap;
}
//---------------------------------------------------------------------

API void Transform_PlaceUnderMouse()
{
	if (!CIDEApp->SelectedEntity.isvalid()) return;

	Event::SetTransform Evt(CIDEApp->SelectedEntity->Get<matrix44>(Attr::Transform));

	vector3 Pos = EnvQueryMgr->GetMousePos3D();

	//!!!DUPLICATE CODE!
	if (CIDEApp->LimitToGround || CIDEApp->SnapToGround)
	{
		int SelfPhysicsID;
		CPropAbstractPhysics* pPhysProp = CIDEApp->SelectedEntity->FindProperty<CPropAbstractPhysics>();

		float LocalMinY = 0.f;
		if (pPhysProp)
		{
			Physics::CEntity* pPhysEnt = pPhysProp->GetPhysicsEntity();
			if (pPhysEnt)
			{
				SelfPhysicsID = pPhysEnt->GetUniqueID();
				
				bbox3 AABB;
				pPhysEnt->GetComposite()->GetAABB(AABB);

				//!!!THIS SHOULDN'T BE THERE! Tfm must be adjusted inside entity that fixes it.
				// I.e. AABB must be translated in GetAABB or smth.
				float PhysEntY = pPhysEnt->GetTransform().pos_component().y;
				float AABBPosY = AABB.center().y;

				LocalMinY -= (AABB.extents().y - AABBPosY + PhysEntY);

				//!!!tmp hack, need more general code!
				if (pPhysEnt->IsA(Physics::CCharEntity::RTTI))
					LocalMinY -= ((Physics::CCharEntity*)pPhysEnt)->Hover;
			}
			else SelfPhysicsID = -1;
		}
		else SelfPhysicsID = -1;

		CEnvInfo Info;
		EnvQueryMgr->GetEnvInfoAt(vector3(Pos.x, Pos.y + 500.f, Pos.z), Info, 1000.f, SelfPhysicsID);
		
		float MinY = Pos.y + LocalMinY;
		if (Info.WorldHeight > MinY || (CIDEApp->SnapToGround && Info.WorldHeight < MinY))
			Pos.y = Info.WorldHeight - LocalMinY;
	}

	Evt.Transform.set_translation(Pos);
	CIDEApp->SelectedEntity->FireEvent(Evt);
}
//---------------------------------------------------------------------

