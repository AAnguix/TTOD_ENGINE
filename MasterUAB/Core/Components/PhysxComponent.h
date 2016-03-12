#ifndef _PHYSXCOMPONENT_H
#define _PHYSXCOMPONENT_H

#include "Components\Component.h"

class CPhysxComponent : public CComponent
{
private:
	bool m_CharacterController;

public:
	CPhysxComponent(const std::string &Name, CRenderableObject *Owner, bool CharacterController);
	virtual ~CPhysxComponent();
	virtual void Update(float ElapsedTime);
	virtual void Render(CRenderManager &RenderManager);
	virtual void RenderDebug(CRenderManager &RenderManager);
	static CPhysxComponent * CreatePhysxComponent(const std::string &Name,CRenderableObject *Owner, bool CharacterController);
};

#endif