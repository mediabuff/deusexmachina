#pragma once
#ifndef __DEM_L1_SCENE_NODE_LOADER_H__
#define __DEM_L1_SCENE_NODE_LOADER_H__

#include <Resources/ResourceCreator.h>

// Loads scene node hierarchy and attributes from DEM "scn" format

namespace Scene
{
	typedef Ptr<class CSceneNode> PSceneNode;
}

namespace Resources
{

class CSceneNodeLoaderSCN: public IResourceCreator
{
	__DeclareClassNoFactory;

public:

	//???!!!TMP!? Doesn't fit very good into a resource system
	Scene::PSceneNode RootNode;	// If not NULL, root node data will be loaded into it instead of allocating a new node

	virtual const Core::CRTTI&			GetResultType() const override;
	virtual PResourceObject				CreateResource(CStrID UID) override;
};

typedef Ptr<CSceneNodeLoaderSCN> PSceneNodeLoaderSCN;

}

#endif
