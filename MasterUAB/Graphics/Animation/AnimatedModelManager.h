#ifndef _ANIMATEDMODELMANAGER_H
#define _ANIMATEDMODELMANAGER_H

#include "Utils\TemplatedMapManager.h"
#include "Animation\AnimatedCoreModel.h"


class CAnimatedModelManager : public CTemplatedMapManager<CAnimatedCoreModel> 
{ 

private:  
	std::string     m_Filename; 
	std::vector<CAnimatedCoreModel *> l_AnimatedCoreModelVector; /*To list a.models in LUA*/

public:  
	CAnimatedModelManager();  
	virtual ~CAnimatedModelManager();  
	void Load(const std::string &Filename);  
	void Reload(); 

	const std::vector<CAnimatedCoreModel *> & GetLUAAnimatedModels();
};

#endif