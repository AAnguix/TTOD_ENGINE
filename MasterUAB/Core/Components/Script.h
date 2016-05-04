#ifndef _SCRIPT_H
#define _SCRIPT_H

#include "Components\Component.h"

class CScript : public CComponent
{

private:
	std::string m_FnOnCreate;
	std::string m_FnOnDestroy;
	std::string m_FnOnUpdate;
	std::string m_FnOnRender;
	std::string m_FnOnRenderDebug;

public:
	static CScript * AddScript(const std::string &Name, CRenderableObject *Owner, const std::string &FnOnCreate, const std::string &FnOnDestroy,
	const std::string &FnOnUpdate, const std::string &FnOnRender, const std::string &FnOnRenderDebug);

	CScript(const std::string &Name, CRenderableObject *Owner, const std::string &FnOnCreate, const std::string &FnOnDestroy, const std::string &FnOnUpdate, const std::string &FnOnRender, const std::string &FnOnRenderDebug);
	virtual ~CScript();
	virtual void Update(float ElapsedTime);
	virtual void Render(CRenderManager &RenderManager);
	virtual void RenderDebug(CRenderManager &RenderManager);
	
};

#endif