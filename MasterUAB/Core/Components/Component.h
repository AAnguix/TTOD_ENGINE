#ifndef _COMPONENT_H
#define _COMPONENT_H

class CGameObject;
class CRenderManager;
class CEmptyPointerClass;
#include "Utils\Named.h"

class CComponent : public CNamed
{

protected:
	CGameObject *m_Owner;
	bool m_Enabled;

public:
	CComponent(const std::string &Name, CGameObject *Owner, bool Enabled = true)
	:CNamed(Name)
	,m_Owner(Owner)
	,m_Enabled(Enabled){};

	virtual ~CComponent() {}
	virtual void Update(float ElapsedTime) = 0;
	virtual void Render(CRenderManager &RenderManager) {}
	virtual void RenderDebug(CRenderManager &RenderManager) {}
	CGameObject* GetOwner(){ return m_Owner; };
	void Enable(){ m_Enabled = true; };
	void Disable(){ m_Enabled = false;};
	bool IsEnabled(){ return m_Enabled; };

	CEmptyPointerClass* GetEnabledLuaAddress() const { return (CEmptyPointerClass *)((void*)&m_Enabled); }
};

#endif