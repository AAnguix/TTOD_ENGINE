#include "Materials\MaterialManager.h"
#include "Utils\Utils.h"
#include "XML\XMLTreeNode.h"
#include "Engine.h"
#include "Log\Log.h"

CMaterialManager::CMaterialManager() : m_Filename("")
{
	
}

CMaterialManager::~CMaterialManager()
{
	Destroy();
}

void CMaterialManager::Destroy()
{
	std::map<const std::string, std::vector<CMaterial*>>::iterator itMap;

	for (itMap = m_MaterialsPerFileName.begin(); itMap != m_MaterialsPerFileName.end(); ++itMap)
	{
		itMap->second.clear();
	}

	CTemplatedMapManager::Destroy();
}

void CMaterialManager::Load(const std::string &Filename)
{
	m_Filename=Filename;

	CXMLTreeNode l_XML;

	if (l_XML.LoadFile(Filename.c_str()))
	{
		CXMLTreeNode l_Materials = l_XML["materials"];
		std::vector<CMaterial*> l_FileMaterials;

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
					else
					{
						l_FileMaterials.push_back(l_Material);
					}
				}
			}

			m_MaterialsPerFileName.insert(std::pair<std::string, std::vector<CMaterial*>>(Filename, l_FileMaterials));
		} 
		else {assert(false); }
	}
	else { assert(false); }
}

void CMaterialManager::Reload()
{
	Destroy();
	std::vector<std::string> l_MaterialsFileNames;
	
	std::map<const std::string, std::vector<CMaterial*>>::iterator itMap;
	for (itMap = m_MaterialsPerFileName.begin(); itMap != m_MaterialsPerFileName.end(); ++itMap)
	{
		std::string l_MaterialFileName = itMap->first;
		l_MaterialsFileNames.push_back(l_MaterialFileName);
	}
	m_MaterialsPerFileName.clear();

	for (size_t i = 0; i < l_MaterialsFileNames.size(); ++i)
	{
		Load(l_MaterialsFileNames[i]);
	}
	l_MaterialsFileNames.clear();

	#ifdef _DEBUG
		CEngine::GetSingleton().GetLogManager()->Log("MaterialManager reloaded");
	#endif
}
 
const std::vector<CMaterial *> & CMaterialManager::GetLUAMaterials()
{
	l_MaterialsVector.clear();
	CTemplatedMapManager<CMaterial>::TMapResource &l_MaterialsMap = GetResourcesMap();
	for (CTemplatedMapManager<CMaterial>::TMapResource::iterator it = l_MaterialsMap.begin(); it != l_MaterialsMap.end(); ++it)
		l_MaterialsVector.push_back(it->second);

	return l_MaterialsVector;
}

const std::vector<CMaterial *> & CMaterialManager::GetLUAFileNameMaterials(const std::string &Filename)
{
	std::map<const std::string, std::vector<CMaterial*>>::iterator itMap;

	itMap = m_MaterialsPerFileName.find(Filename);
	if (itMap == m_MaterialsPerFileName.end())
	{
		itMap = m_MaterialsPerFileName.find("./"+Filename);
		if (itMap == m_MaterialsPerFileName.end())
			assert(false);
	}
	return itMap->second;
}