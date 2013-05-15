#include "LODGroup.h"

#include <Scene/Scene.h>
#include <IO/BinaryReader.h>

namespace Scene
{
__ImplementClass(Scene::CLODGroup, 'LODG', Scene::CSceneNodeAttr);

bool CLODGroup::LoadDataBlock(nFourCC FourCC, IO::CBinaryReader& DataReader)
{
	switch (FourCC)
	{
		case 'HSRT': // TRSH
		{
			//!!!node ID!

			short Count;
			if (!DataReader.Read(Count)) FAIL;
			for (short i = 0; i < Count; ++i)
			{
				float SqThreshold;
				DataReader.Read<float>(SqThreshold);
				SqThreshold *= SqThreshold;
				//SqThresholds.Add(SqThreshold, ChildID);
				//!!!it is normal to have CStrID::Empty as ID - LOD assumes no children!
			}
			OK;
		}
		case 'NIMD': // DMIN
		{
			DataReader.Read<float>(MinSqDistance);
			MinSqDistance *= MinSqDistance;
			OK;
		}
		case 'XAMD': // DMAX
		{
			DataReader.Read<float>(MaxSqDistance);
			MaxSqDistance *= MaxSqDistance;
			OK;
		}
		default: FAIL;
	}
}
//---------------------------------------------------------------------

void CLODGroup::Update()
{
	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//???what if camera transform wasn't updated this frame yet?
	//Update camera branch of scene graph before?
	//Can use Updated(ThisFrame) flag and clear at each frame ending, with clearing visible meshes and lights
	//Flag Changed(ThisFrame) is smth different but may be usable too

	if (!pNode || !pNode->GetScene()->GetMainCamera()) return;

	vector3 DistanceVector = pNode->GetWorldPosition() -
		pNode->GetScene()->GetMainCamera()->GetNode()->GetWorldPosition();
	float SqDist = DistanceVector.lensquared();

	CStrID SelectedChild;
	if (SqDist >= MinSqDistance && SqDist <= MaxSqDistance)
		for (int i = 0; i < SqThresholds.GetCount(); ++i)
			if (SqThresholds.KeyAtIndex(i) > SqDist) SelectedChild = SqThresholds.ValueAtIndex(i);
			else break;

	for (DWORD i = 0; i < pNode->GetChildCount(); ++i)
	{
		CSceneNode& Node = *pNode->GetChild(i);
		if (Node.IsLODDependent())
			Node.Activate(Node.GetName() == SelectedChild);
	}
}
//---------------------------------------------------------------------

}