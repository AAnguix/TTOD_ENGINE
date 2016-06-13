#ifndef _SCRIPT_H
#define _SCRIPT_H

#include "Components\Component.h"
class CLUAComponent;

class CScript : public CComponent
{

private:
	CLUAComponent* m_LuaComponent;

public:
	CLUAComponent* GetLuaComponent() const{ return m_LuaComponent; };
	CScript(const std::string& Name, CGameObject* Owner, CLUAComponent* Component);
	virtual ~CScript();
	void Update(float ElapsedTime);
	
};

#endif