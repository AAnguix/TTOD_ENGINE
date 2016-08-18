#ifndef _SCRIPTAMANAGER_H
#define _SCRIPTAMANAGER_H

#include <vector>
class CScript;
class CLuaGameObjectHandle;
class CLUAComponent;
class CGameObject;

class CScriptManager
{
private:
	std::vector<CScript*> m_Components;
	void UpdateComponents(float ElapsedTime);

public:
	CScriptManager();
	virtual ~CScriptManager();
	void Update(float ElapsedTime);
	CScript* AddComponent(const std::string& Name, CLuaGameObjectHandle* Owner, CLUAComponent* Component);
	void RemoveComponent(CGameObject* Owner);
	void RemoveComponents();
	CScript* GetScript(const std::string& ScriptName) const;
	const std::vector<CScript *> & GetLUAScripts();
};

#endif