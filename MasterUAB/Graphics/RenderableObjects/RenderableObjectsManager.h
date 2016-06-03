#ifndef _RENDERABLEOBJECTSMANAGER_H
#define _RENDERABLEOBJECTSMANAGER_H

#include "RenderableObjects\RenderableObject.h"
#include "Utils\TemplatedVectorMapManager.h"
class CRenderManager;

class CRenderableObjectsManager : public CTemplatedVectorMapManager<CRenderableObject>, public CNamed
{

private:
	std::string m_Filename;

public:
	CRenderableObjectsManager();
	virtual ~CRenderableObjectsManager();
	//void Reload();
	void Update(float ElapsedTime);
	void Render(CRenderManager *RM);
	
};

#endif