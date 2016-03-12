#include "StaticMeshes\StaticMeshManager.h"
#include "Utils\Utils.h"

CStaticMeshManager::CStaticMeshManager(){m_FileName="";}

CStaticMeshManager::~CStaticMeshManager(){}

bool CStaticMeshManager::Load(const std::string &FileName)  
{
	m_FileName=FileName;

	CXMLTreeNode l_XML;
	
	if (l_XML.LoadFile(m_FileName.c_str()))
	{
		CXMLTreeNode l_StaticMeshes = l_XML["static_meshes"];

		if (l_StaticMeshes.Exists())
		{
			for (int i = 0; i < l_StaticMeshes.GetNumChildren(); ++i)
			{
				CXMLTreeNode l_Element = l_StaticMeshes(i);

				if (l_Element.GetName() == std::string("static_mesh"))
				{
					CStaticMesh *l_StaticMesh=new CStaticMesh();
					l_StaticMesh->SetName(l_Element.GetPszProperty("name"));

					if(!AddResource(l_StaticMesh->GetName(), l_StaticMesh))
					{
						CHECKED_DELETE(l_StaticMesh);
					}
					else
					{
						l_StaticMesh->Load(l_Element.GetPszProperty("filename"));
					}
				}
			}
		}
		return true;
	}
	return false;
	
}

bool CStaticMeshManager::Reload()
{
	Destroy();
	return Load(m_FileName);
}

const std::vector<CStaticMesh *> & CStaticMeshManager::GetLUAStaticMeshes()
{
	l_StaticMeshVector.clear();
	CTemplatedMapManager<CStaticMesh>::TMapResource &l_EffectsMap=GetResourcesMap();
	for(CTemplatedMapManager<CStaticMesh>::TMapResource::iterator it=l_EffectsMap.begin();it!=l_EffectsMap.end();++it)
		l_StaticMeshVector.push_back(it->second);

	return l_StaticMeshVector;
}