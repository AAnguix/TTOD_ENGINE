#include "StaticMeshes\StaticMeshManager.h"
#include "Utils\Utils.h"
#include "XML\XMLTreeNode.h"
#include "Engine\Engine.h"
#include "Log\Log.h"
#include "PhysXManager.h"

CStaticMeshManager::CStaticMeshManager()
:m_FileName("")
{
}

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
					CStaticMesh *l_StaticMesh = new CStaticMesh(l_Element);

					if(!AddResource(l_StaticMesh->GetName(), l_StaticMesh))
					{
						CHECKED_DELETE(l_StaticMesh);
					}
					else
					{
						bool l_Load = l_StaticMesh->Load(l_Element.GetPszProperty("filename"));
						#ifdef _DEBUG
							if (!l_Load)
							{
								std::string l_Name = l_Element.GetPszProperty("name");
								CEngine::GetSingleton().GetLogManager()->Log("Mesh " + l_Name + " Not loaded properly");
							}
						#endif
						assert(l_Load);
						if (l_Load)
							l_StaticMesh->LoadShape(l_Element.GetPszProperty("shape_type", ""), l_Element.GetPszProperty("group", ""), l_Element.GetBoolProperty("is_exclusive", false));
					}
				}
				if (l_Element.GetName() == std::string("physx_shape"))
				{
					std::string l_Name = l_Element.GetPszProperty("name");
					std::string l_Group = l_Element.GetPszProperty("group");
					std::string l_Filename = l_Element.GetPszProperty("filename");
					Vect3f  l_Position = l_Element.GetVect3fProperty("pos", v3fZERO);
					float l_Yaw = l_Element.GetFloatProperty("yaw", 0.0f);
					float l_Pitch = l_Element.GetFloatProperty("pitch", 0.0f);
					float l_Roll = l_Element.GetFloatProperty("roll", 0.0f);
					CEngine::GetSingleton().GetPhysXManager()->CreateTriangleMeshFromFile(l_Name, l_Filename, l_Position, l_Yaw, l_Pitch, l_Roll, "PhXMesh", 10.0f, 10.0f, 0.5f, l_Group);
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
	bool l_Result = Load(m_FileName);
	#ifdef _DEBUG
		CEngine::GetSingleton().GetLogManager()->Log("StaticMeshManager reloaded");
	#endif	
	return l_Result;
}

const std::vector<CStaticMesh *> & CStaticMeshManager::GetLUAStaticMeshes()
{
	l_StaticMeshVector.clear();
	CTemplatedMapManager<CStaticMesh>::TMapResource &l_EffectsMap=GetResourcesMap();
	for(CTemplatedMapManager<CStaticMesh>::TMapResource::iterator it=l_EffectsMap.begin();it!=l_EffectsMap.end();++it)
		l_StaticMeshVector.push_back(it->second);

	return l_StaticMeshVector;
}