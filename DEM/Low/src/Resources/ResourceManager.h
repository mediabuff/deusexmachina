#pragma once
#include <Data/Ptr.h>
#include <Data/StringID.h>
#include <Data/HashTable.h>
#include <Data/Singleton.h>
#include <vector>

// Resource manager controls resource loading, lifetime, uniquity, and serves as
// a hub for accessing any types of assets in an abstract way.
// Resource ID is either an engine path to the file or an abstract string.

namespace Core
{
	class CRTTI;
}

namespace IO
{
	class CIOServer;
	typedef Ptr<class CStream> PStream;
}

namespace Resources
{
typedef Ptr<class CResource> PResource;
typedef Ptr<class IResourceCreator> PResourceCreator;

#define ResourceMgr Resources::CResourceManager::Instance()

class CResourceManager
{
	__DeclareSingleton(CResourceManager);

protected:

	struct CDefaultCreatorRecord
	{
		CStrID				Extension;
		const Core::CRTTI*	pRsrcType;
		PResourceCreator	Creator;
	};

	IO::CIOServer*						pIO = nullptr;
	CHashTable<CStrID, PResource>		Registry;
	std::vector<CDefaultCreatorRecord>	DefaultCreators;

public:

	CResourceManager(IO::CIOServer* pIOServer, UPTR InitialCapacity = 256);
	~CResourceManager();

	bool				RegisterDefaultCreator(const char* pFmtExtension, const Core::CRTTI* pRsrcType, IResourceCreator* pCreator);
	PResourceCreator	GetDefaultCreator(const char* pFmtExtension, const Core::CRTTI* pRsrcType = nullptr);
	template<class TRsrc>
	PResourceCreator	GetDefaultCreatorFor(const char* pFmtExtension) { return GetDefaultCreator(pFmtExtension, &TRsrc::RTTI); }

	template<class TRsrc>
	PResource			RegisterResource(const char* pUID) { return RegisterResource(pUID, TRsrc::RTTI); }
	PResource			RegisterResource(const char* pUID, const Core::CRTTI& RsrcType);
	PResource			RegisterResource(const char* pUID, IResourceCreator* pCreator);
	CResource*			FindResource(const char* pUID) const;
	CResource*			FindResource(CStrID UID) const;
	void				UnregisterResource(const char* pUID);
	void				UnregisterResource(CStrID UID);

	IO::PStream			CreateResourceStream(const char* pUID, const char*& pOutSubId);
};

}
