#pragma once
#ifndef __DEM_L1_PHYSICS_COLLISION_SHAPE_LOADER_H__
#define __DEM_L1_PHYSICS_COLLISION_SHAPE_LOADER_H__

#include <Resources/ResourceCreator.h>

// Loads a collision shape from HRD or PRM

namespace Resources
{

class CCollisionShapeLoaderPRM: public IResourceCreator
{
	__DeclareClass(CCollisionShapeLoader);

public:

	virtual ~CCollisionShapeLoaderPRM() {}

	virtual const Core::CRTTI&			GetResultType() const override;
	virtual PResourceObject				CreateResource(CStrID UID) override;
};

typedef Ptr<CCollisionShapeLoaderPRM> PCollisionShapeLoaderPRM;

}

#endif
