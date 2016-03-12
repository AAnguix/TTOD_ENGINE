#ifndef _COMPONENTMANAGER_H
#define _COMPONENTMANAGER_H

#include "Utils\TemplatedVectorMapManager.h"
#include "Components\Component.h"
#include "Render\RenderManager.h"

class CComponentManager : public CTemplatedVectorMapManager<CComponent>
{

public:
	CComponentManager();
	virtual ~CComponentManager();
	void Update(float ElapsedTime);
	void Render(CRenderManager &RenderManager);
	void RenderDebug(CRenderManager &RenderManager);
	bool AddComponent(CComponent *Component);
};

#endif