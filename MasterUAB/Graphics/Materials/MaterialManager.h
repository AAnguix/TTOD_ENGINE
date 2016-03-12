#ifndef _MATERIALMANAGER_H
#define _MATERIALMANAGER_H

#include "Utils\TemplatedMapManager.h" 
#include "Materials\Material.h"

class CMaterialManager : public CTemplatedMapManager<CMaterial> 
{ 

private: 
	std::string m_Filename; 
	std::vector<CMaterial*> l_MaterialsVector;

public: 
	CMaterialManager();  
	virtual ~CMaterialManager();  
	void Load(const std::string &Filename);  
	void Reload(); 

	const std::vector<CMaterial *> & GetLUAMaterials();
};

#endif