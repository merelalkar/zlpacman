#ifndef PEGAS_RESOURCE_MANAGER_H
#define PEGAS_RESOURCE_MANAGER_H
#pragma once

namespace Pegas
{
	template<class ResourceCode>
	class BaseResource
	{
	public:
		BaseResource(): m_izInitialized(false) {};
		virtual ~BaseResource() {};

		virtual void create(const ResourceCode& code) = 0;
		virtual void load() = 0;
		virtual void destroy() = 0;

	protected:
		ResourceCode m_resourceCode;
		bool m_izInitialized;

	private:
		BaseResource(const BaseResource<ResourceCode>& src);
		BaseResource<ResourceCode>& operator=(const BaseResource<ResourceCode>& src);
	};


	template<class ResourceType, class ResourceCode>
	class ResourceManager: public Singleton<ResourceManager<ResourceType, ResourceCode> >
	{
	public:
		typedef std::map<RESOURCEID, ResourceType*> RESOURCE_LOOKUP_MAP;

		ResourceManager(): Singleton(*this) {};
		virtual ~ResourceManager() { destroyAll(); };

		ResourceType* registerResource(RESOURCEID id, const ResourceCode& platformCode);
		ResourceType* getResource(RESOURCEID id);
		void load(RESOURCEID id);
		void loadAll();
		void destroy(RESOURCEID id);
		void destroyAll();

	private:
		ResourceManager(const ResourceManager<ResourceType, ResourceCode>& src);
		ResourceManager<ResourceType, ResourceCode>& operator=(const ResourceManager<ResourceType, ResourceCode>& src);

		RESOURCE_LOOKUP_MAP m_resourceMap;
	};

	template<class ResourceType, class ResourceCode>
	inline ResourceType* 
	ResourceManager<ResourceType, ResourceCode>::registerResource(RESOURCEID id, 
																const ResourceCode& platformCode)
	{
		if(m_resourceMap.count(id) > 0)
		{
			throw std::out_of_range("Resource with id already exist");
		}
		ResourceType* pResource = new ResourceType();
		pResource->create(platformCode);
		
		m_resourceMap.insert(std::make_pair(id, pResource));

		return pResource;
	}
	
	template<class ResourceType, class ResourceCode>
	ResourceType* 
	ResourceManager<ResourceType, ResourceCode>::getResource(RESOURCEID id)
	{
		if(m_resourceMap.count(id) == 0)
		{
			throw std::out_of_range("Resource with id is not exist");
		}

		return m_resourceMap[id];		
	}

	template<class ResourceType, class ResourceCode>
	void ResourceManager<ResourceType, ResourceCode>::load(RESOURCEID id)
	{
		ResourceType* pResource = getResource(id);
		pResource->load();		
	}
	
	template<class ResourceType, class ResourceCode>
	void ResourceManager<ResourceType, ResourceCode>::loadAll()
	{
		for(RESOURCE_LOOKUP_MAP::iterator it = m_resourceMap.begin(); 
			it != m_resourceMap.end(); ++it)
		{
			ResourceType* pResource = it->second;
			pResource->load();
		}
	}
	
    template<class ResourceType, class ResourceCode>
	void ResourceManager<ResourceType, ResourceCode>::destroy(RESOURCEID id)
	{
		ResourceType* pResource = getResource(id);
		pResource->destroy();
		m_resourceMap.erase(id);		
	}
	
    template<class ResourceType, class ResourceCode>
	void ResourceManager<ResourceType, ResourceCode>::destroyAll()
	{
		for(RESOURCE_LOOKUP_MAP::iterator it = m_resourceMap.begin(); 
			it != m_resourceMap.end(); ++it)
		{
			ResourceType* pResource = it->second;
			pResource->destroy();
		}
		m_resourceMap.clear();
	}
}

#endif //PEGAS_RESOURCE_MANAGER_H