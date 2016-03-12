#include "Materials\MaterialManager.h"
#include "Utils\Utils.h"

CMaterialManager::CMaterialManager() : m_Filename("")
{
	
}

CMaterialManager::~CMaterialManager()
{

}

void CMaterialManager::Load(const std::string &Filename)
{
	m_Filename=Filename;

	CXMLTreeNode l_XML;

	if (l_XML.LoadFile(Filename.c_str()))
	{
		CXMLTreeNode l_Materials = l_XML["materials"];

		if (l_Materials.Exists())
		{
			for (int i = 0; i < l_Materials.GetNumChildren(); ++i)
			{
				CXMLTreeNode l_Element = l_Materials(i);

				if (l_Element.GetName() == std::string("material"))
				{
					CMaterial *l_Material=new CMaterial(l_Element);

					if(!AddResource(l_Material->GetName(), l_Material))
					{
						CHECKED_DELETE(l_Material);
					}
				}
			}
		}
	}
}

void CMaterialManager::Reload()
{
	Destroy();
	Load(m_Filename);
}
 
const std::vector<CMaterial *> & CMaterialManager::GetLUAMaterials()
{
	l_MaterialsVector.clear();
	CTemplatedMapManager<CMaterial>::TMapResource &l_MaterialsMap = GetResourcesMap();
	for (CTemplatedMapManager<CMaterial>::TMapResource::iterator it = l_MaterialsMap.begin(); it != l_MaterialsMap.end(); ++it)
		l_MaterialsVector.push_back(it->second);

	return l_MaterialsVector;
}