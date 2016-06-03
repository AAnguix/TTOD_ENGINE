#ifndef _COMPONENT_H
#define _COMPONENT_H

class CRenderableObject;
class CRenderManager;
#include "Utils\Named.h"

class CComponent : public CNamed
{

protected:
	CRenderableObject *m_Owner;
	bool m_Enabled;

public:
	CComponent(const std::string &Name, CRenderableObject *Owner) 
	:CNamed(Name)
	,m_Owner(Owner)
	,m_Enabled(true){};

	virtual ~CComponent() {}
	virtual void Update(float ElapsedTime) = 0;
	virtual void Render(CRenderManager &RenderManager) {}
	virtual void RenderDebug(CRenderManager &RenderManager) {}
	CRenderableObject* GetOwner(){ return m_Owner; };
	void Enable(){ m_Enabled = true; };
	void Disable(){ m_Enabled = false;};
	bool IsEnabled(){ return m_Enabled; };
};

#endif