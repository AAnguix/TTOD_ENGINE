#ifndef _ANIMATEDCOREMODEL_H
#define _ANIMATEDCOREMODEL_H

#include "Utils\Named.h"
#include <vector>
#include "Math\Vector3.h"
#include "Components\AnimatorController\Animation.h"
class CMaterial;
class CalCoreModel;

class CAnimatedCoreModel : public CNamed 
{ 

private:  
	/*Cal3D Model interface*/
	CalCoreModel *m_CalCoreModel;  
	std::vector<CMaterial *> m_Materials;  
	std::vector<unsigned int> m_VertexTypes;
	std::string m_Path;
	std::vector<EAnimation> m_Animations;
	Vect3f m_BSPosition;  
	Vect3f m_BSRadius; 

	bool LoadMesh(const std::string &Filename);  
	bool LoadSkeleton(const std::string &Filename);  
	bool LoadAnimation(const std::string &Name, const std::string &Filename, bool Loop, float Weight, bool AutoLock);

public: 
	CAnimatedCoreModel();  
	virtual ~CAnimatedCoreModel();  
	CalCoreModel *GetCoreModel();  
	EAnimation GetAnimation(const std::string &Name);
	const std::vector<CMaterial*> & GetMaterials() const {return m_Materials;}  
	const std::vector<unsigned int> & GetVertexTypes() const { return m_VertexTypes; }
	void Load(const std::string &Path);
	int GetBoneId(const std::string &BoneName);
}; 

#endif