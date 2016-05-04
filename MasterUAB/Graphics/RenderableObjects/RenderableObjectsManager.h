#ifndef _RENDERABLEOBJECTSMANAGER_H
#define _RENDERABLEOBJECTSMANAGER_H

#include "RenderableObjects\RenderableObject.h"
#include "Utils\TemplatedVectorMapManager.h"
#include "Render\RenderManager.h"

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
	

	/*void Load(const std::string &FileName);*/
	
};

#endif