#ifndef _STATICMESHMANAGER_H
#define _STATICMESHMANAGER_H

#include "Utils\TemplatedMapManager.h"
#include "StaticMeshes\StaticMesh.h"

class CStaticMeshManager : public CTemplatedMapManager<CStaticMesh> 
{

protected:  
	std::string    m_FileName; 

private:
	std::vector<CStaticMesh *> l_StaticMeshVector; /*To list s.mesh in LUA*/

public:
	CStaticMeshManager();  
	virtual ~CStaticMeshManager();  
	bool Load(const std::string &FileName);  
	bool Reload(); 

	const std::vector<CStaticMesh *> & GetLUAStaticMeshes();
}; 

#endif 