#ifndef _CTEMPLATEDVECTORMAPMANAGER_H
#define _CTEMPLATEDVECTORMAPMANAGER_H

#include <map>
#include <vector>
#include <cassert>

template<class T>
class CTemplatedVectorMapManager
{

public:
	class CMapResourceValue
	{
		public:
			T *m_Value;
			size_t m_Id;
			CMapResourceValue(T *Value, size_t Id): m_Value(Value), m_Id(Id){};
			CMapResourceValue()
				: m_Value(NULL)
				, m_Id(0)
			{};
	};

	typedef std::vector<T *> TVectorResources;

	typedef std::map<std::string, CMapResourceValue> TMapResources;

protected:
	TVectorResources m_ResourcesVector;
	TMapResources m_ResourcesMap;

	public:

		CTemplatedVectorMapManager()
		{
		}
		virtual ~CTemplatedVectorMapManager(){Destroy();}

		bool RemoveResource(const std::string &Name)
		{
			std::map<std::string, CMapResourceValue>::iterator itMap;
			itMap=m_ResourcesMap.find(Name);

			if(itMap != m_ResourcesMap.end())
			{
				size_t id=itMap->second.m_Id;

				m_ResourcesVector.erase(m_ResourcesVector.begin()+id);
				delete itMap->second.m_Value;
				m_ResourcesMap.erase(Name);

				for (itMap = m_ResourcesMap.find(Name); itMap != m_ResourcesMap.end(); ++itMap)
				{
					if(itMap->second.m_Id > id)
						itMap->second.m_Id--;
				}

				return true;
			}
			
			return false;
		}

		virtual T * GetResourceById(size_t Id){return m_ResourcesVector[Id];}

		virtual T * GetResource(const std::string &Name)
		{
			std::map<std::string, CMapResourceValue>::iterator itMap;

			itMap=m_ResourcesMap.find(Name);

			if(itMap == m_ResourcesMap.end())
				return nullptr;
			else
				return itMap->second.m_Value;
		}

		virtual bool AddResource(const std::string &Name, T *Resource)
		{
			std::map<std::string, CMapResourceValue>::iterator itMap;

			itMap=m_ResourcesMap.find(Name);

			if(itMap != m_ResourcesMap.end())
			{
				return false;
			}
			else
			{
				m_ResourcesMap[Name]=CMapResourceValue(Resource, m_ResourcesVector.size());

				m_ResourcesVector.push_back(Resource);

				return true;
			}
		}
		
		virtual void Destroy()
		{
			std::map<std::string, CMapResourceValue>::iterator itMap;
			
			for ( itMap = m_ResourcesMap.begin(); itMap != m_ResourcesMap.end(); ++itMap)
			{
				delete itMap->second.m_Value;
			}

			m_ResourcesMap.clear();
			m_ResourcesVector.clear();
		}

		TMapResources & GetResourcesMap(){return m_ResourcesMap;}

		TVectorResources & GetResourcesVector(){return m_ResourcesVector;}
};

#endif 