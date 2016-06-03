#ifndef _SCRIPT_H
#define _SCRIPT_H

#include "Components\Component.h"
class CLUAComponent;

class CScript : public CComponent
{

private:
	CLUAComponent* m_Component;

public:
	CLUAComponent* GetLuaComponent() const{ return m_Component; };
	CScript(const std::string& Name, CRenderableObject* Owner, CLUAComponent* Component);
	virtual ~CScript();
	void Update(float ElapsedTime);
	
};

#endif