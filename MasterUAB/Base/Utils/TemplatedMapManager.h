#ifndef _CTEMPLATEDMAPMANAGER_H
#define _CTEMPLATEDMAPMANAGER_H

#include <map>
#include <cassert>
#include "Utils\Utils.h"

template<class T>
class CTemplatedMapManager
{
	public:
	typedef std::map<std::string,T*> TMapResource;

	protected:
		
		TMapResource	m_Resources;

	public:
		CTemplatedMapManager(){}
		virtual ~CTemplatedMapManager(){Destroy();}
		virtual T * GetResource(const std::string &Name)
		{
			std::map<std::string, T*>::iterator itMap;

			itMap=m_Resources.find(Name);
			if(itMap == m_Resources.end())
				return nullptr;
			else
				return itMap->second;
		}
		virtual bool AddResource(const std::string &Name, T *Resource)
		{
			std::map<std::string, T*>::iterator itMap;

			itMap=m_Resources.find(Name);

			if(itMap != m_Resources.end())
			{
				return false;
			}
			else
			{
				//m_Resources[Name]=Resource;
				m_Resources.insert(std::pair<std::string,T *>(Name,Resource));
				return true;
			}
		}
		
		virtual void Destroy()
		{
			std::map<std::string, T*>::iterator itMap;
			
			for ( itMap = m_Resources.begin(); itMap != m_Resources.end(); ++itMap)
			{
				delete itMap->second;
			}

			m_Resources.clear();
		}

		TMapResource & GetResourcesMap(){return m_Resources;}
};

#endif 