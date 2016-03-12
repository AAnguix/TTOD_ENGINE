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
	void Reload();
	void Update(float ElapsedTime);
	void Render(CRenderManager *RM);
	CRenderableObject * AddMeshInstance(const std::string &CoreMeshName, const std::string &InstanceName, const Vect3f &Position);
	CRenderableObject * AddAnimatedInstanceModel(const std::string &CoreModelName, const std::string &InstanceModelName, const Vect3f &Position);
	void Load(const std::string &FileName);
	
};

#endif