#ifndef _COMPONENT_H
#define _COMPONENT_H

class CRenderableObject;
#include "Utils\Named.h"
class CRenderManager;
#include "Utils\Utils.h"

class CComponent : public CNamed
{

protected:
	CRenderableObject *m_Owner;

public:
	CComponent(const std::string &Name, CRenderableObject *Owner) : CNamed(Name), m_Owner(Owner){};
	virtual ~CComponent() {}
	virtual void Update(float ElapsedTime) {}
	virtual void Render(CRenderManager &RenderManager) {}
	virtual void RenderDebug(CRenderManager &RenderManager) {}
	UAB_GET_PROPERTY_POINTER(CRenderableObject, Owner);
};

#endif