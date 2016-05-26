#ifndef _MATERIALMANAGER_H
#define _MATERIALMANAGER_H

#include "Utils\TemplatedMapManager.h" 
#include "Materials\Material.h"

class CMaterialManager : public CTemplatedMapManager<CMaterial> 
{ 

private: 
	std::string m_Filename; 
	std::vector<CMaterial*> l_MaterialsVector;
	std::map<const std::string, std::vector<CMaterial*>> m_MaterialsPerFileName;

public: 
	CMaterialManager();  
	virtual ~CMaterialManager();  
	void Load(const std::string &Filename);  
	void Reload(); 

	const std::vector<CMaterial *> & GetLUAMaterials();
	const std::vector<CMaterial *> & GetLUAFileNameMaterials(const std::string &Filename);
};

#endif