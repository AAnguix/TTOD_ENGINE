#ifndef _CTEMPLATEDVECTORMAPMANAGER_H
#define _CTEMPLATEDVECTORMAPMANAGER_H

#include <map>
#include <vector>
//#include <cassert>
#include <assert.h>

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
			:m_Value(NULL)
			,m_Id(0)
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
			CMapResourceValue l_ResourceValue = m_ResourcesMap[Name];
			size_t l_Index = l_ResourceValue.m_Id;
			delete m_ResourcesVector[l_Index];
			m_ResourcesMap.erase(Name);
			m_ResourcesVector.erase(m_ResourcesVector.begin() + l_Index);
			for (TMapResources::iterator l_It = m_ResourcesMap.begin(); l_It != m_ResourcesMap.end(); l_It++)
			{
				if (l_It->second.m_Id>l_Index)
				{
					l_It->second.m_Id--;
				}
			}
			return true;
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