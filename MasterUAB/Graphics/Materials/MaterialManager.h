#ifndef _MATERIALMANAGER_H
#define _MATERIALMANAGER_H

#include "Utils\TemplatedMapManager.h" 
#include "Materials\Material.h"

class CMaterialManager : public CTemplatedMapManager<CMaterial> 
{ 

private: 
	std::string m_Filename; 
	std::vector<CMaterial*> m_LuaMaterialsVector;
	std::map<const std::string, std::vector<CMaterial*>> m_MaterialsPerFileName;

	bool m_EffectsMaterialsLoaded;
	bool m_GuiMaterialsLoaded;

public: 
	CMaterialManager();  
	virtual ~CMaterialManager();  
	void Load(const std::string &Filename);  
	void Reload(); 
	void Destroy();

	//Animated Models materials
	void AddMaterialsFileName(const std::string &MaterialsFileName);
	bool InsertMaterialIntoMaterialsFileName(const std::string &MaterialName, const std::string &MaterialsFileName);

	const std::vector<CMaterial *> & GetLUAMaterials();
	const std::vector<CMaterial *> & GetLUAFileNameMaterials(const std::string &Filename);
};

#endif