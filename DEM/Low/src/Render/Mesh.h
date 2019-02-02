#pragma once
#include <Resources/ResourceObject.h>
#include <Render/RenderFwd.h>
#include <Render/VertexBuffer.h>
#include <Render/IndexBuffer.h>

// GPU mesh resource. It's VB & IB data reside in VRAM in most cases.

namespace Render
{
typedef Ptr<class CMeshData> PMeshData;

class CMesh: public Resources::CResourceObject
{
	__DeclareClass(CMesh);

protected:

	PMeshData		MeshData;
	PVertexBuffer	VB;
	PIndexBuffer	IB;

	// For sharing GPU buffers between multiple meshes
	//UPTR VBOffset;
	//UPTR IBOffset;

public:

	CMesh();
	virtual ~CMesh();

	bool					Create(PMeshData Data, PVertexBuffer VertexBuffer, PIndexBuffer IndexBuffer);
	void					Destroy();

	virtual bool			IsResourceValid() const { return !!VB; }

	PVertexBuffer			GetVertexBuffer() const { return VB; }
	PIndexBuffer			GetIndexBuffer() const { return IB; }
	const CPrimitiveGroup*	GetGroup(UPTR SubMeshIdx, UPTR LOD = 0) const;
};

typedef Ptr<CMesh> PMesh;

}
