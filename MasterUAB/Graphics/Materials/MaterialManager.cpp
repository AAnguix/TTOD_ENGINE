#include "Materials\MaterialManager.h"
#include "Utils\Utils.h"
#include "XML\XMLTreeNode.h"
#include "Engine\Engine.h"
#include "Log\Log.h"
#include "Engine\EngineSettings.h"

CMaterialManager::CMaterialManager()
:m_Filename("")
,m_EffectsMaterialsLoaded(false)
,m_GuiMaterialsLoaded(false)
{
	
}

CMaterialManager::~CMaterialManager()
{
	CTemplatedMapManager::Destroy();
}

void CMaterialManager::Destroy()
{
	std::vector<std::string> m_RemovedFileNames;

	std::map<const std::string, std::vector<CMaterial*>>::iterator itMap;
	for (itMap = m_MaterialsPerFileName.begin(); itMap != m_MaterialsPerFileName.end(); ++itMap)
	{
		std::string l_GuiMaterials = CEngine::GetSingleton().GetEngineSettings()->GetGuiMaterialsFileName();
		std::string l_EffectsMaterials = CEngine::GetSingleton().GetEngineSettings()->GetEffectsMaterialsFileName();

		if (itMap->first != l_GuiMaterials && itMap->first != l_EffectsMaterials)
		{
			std::vector<CMaterial*> l_MaterialsVector = itMap->second;

			for (size_t i = 0; i < l_MaterialsVector.size() ; ++i)
			{
				std::map<std::string, CMaterial*>::iterator itr = m_Resources.find(l_MaterialsVector[i]->GetName());
				if (itr != m_Resources.end())
				{
					delete itr->second;
					m_Resources.erase(itr);
				}
			}
			l_MaterialsVector.clear();

			m_RemovedFileNames.push_back(itMap->first);
		}
	}

	for (size_t i = 0; i < m_RemovedFileNames.size(); ++i)
	{
		m_MaterialsPerFileName.erase(m_RemovedFileNames[i]);
	}
	m_RemovedFileNames.clear();
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

			std::string l_GuiMaterials = CEngine::GetSingleton().GetEngineSettings()->GetGuiMaterialsFileName();
			std::string l_EffectsMaterials = CEngine::GetSingleton().GetEngineSettings()->GetEffectsMaterialsFileName();

			if (!m_GuiMaterialsLoaded && Filename == l_GuiMaterials) m_GuiMaterialsLoaded = true;
			if (!m_EffectsMaterialsLoaded && Filename == l_EffectsMaterials) m_EffectsMaterialsLoaded = true;

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

void CMaterialManager::AddMaterialsFileName(const std::string &MaterialsFileName)
{
	std::map<const std::string, std::vector<CMaterial*>>::iterator it;
	std::vector<CMaterial*> l_FileMaterials;
	for (it = m_MaterialsPerFileName.begin(); it != m_MaterialsPerFileName.end(); ++it)
	{
		if (it->first == MaterialsFileName)
			return;
	}
	m_MaterialsPerFileName.insert(std::pair<std::string, std::vector<CMaterial*>>(MaterialsFileName, l_FileMaterials));
}

bool CMaterialManager::InsertMaterialIntoMaterialsFileName(const std::string &MaterialName, const std::string &MaterialsFileName)
{
	std::map<const std::string, std::vector<CMaterial*>>::iterator it;
	for (it = m_MaterialsPerFileName.begin(); it != m_MaterialsPerFileName.end(); ++it)
	{
		if (it->first == MaterialsFileName)
		{
			CMaterial* l_Material = GetResource(MaterialName);
			assert(l_Material != nullptr);
			it->second.push_back(l_Material);
			return true;
		}
	}
	return false;
}
 
const std::vector<CMaterial *> & CMaterialManager::GetLUAMaterials()
{
	m_LuaMaterialsVector.clear();
	CTemplatedMapManager<CMaterial>::TMapResource &l_MaterialsMap = GetResourcesMap();
	for (CTemplatedMapManager<CMaterial>::TMapResource::iterator it = l_MaterialsMap.begin(); it != l_MaterialsMap.end(); ++it)
		m_LuaMaterialsVector.push_back(it->second);

	return m_LuaMaterialsVector;
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