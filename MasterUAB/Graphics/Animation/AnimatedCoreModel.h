#ifndef _ANIMATEDCOREMODEL_H
#define _ANIMATEDCOREMODEL_H

#include "Utils\Named.h"
#include <vector>
class CMaterial;
#include "cal3d\coremodel.h"

class CAnimatedCoreModel : public CNamed 
{ 

private:  
	CalCoreModel   *m_CalCoreModel;  
	std::vector<CMaterial *> m_Materials;  
	std::string   m_Path;  
	Vect3f    m_BSPosition;  
	Vect3f    m_BSRadius;  

	bool LoadMesh(const std::string &Filename);  
	bool LoadSkeleton(const std::string &Filename);  
	bool LoadAnimation(const std::string &Name, const std::string &Filename);

public: 
	CAnimatedCoreModel();  
	virtual ~CAnimatedCoreModel();  
	CalCoreModel *GetCoreModel();  
	const std::vector<CMaterial *> & GetMaterials() const {return m_Materials;}  
	void Load(const std::string &Path); 
}; 

#endif