#ifndef _SCRIPTAMANAGER_H
#define _SCRIPTAMANAGER_H

#include <vector>
class CScript;
class CGameObject;
class CLUAComponent;

class CScriptManager
{
private:
	std::vector<CScript*> m_Components;
	void UpdateComponents(float ElapsedTime);

public:
	CScriptManager();
	virtual ~CScriptManager();
	void Update(float ElapsedTime);
	CScript* AddComponent(const std::string& Name, CGameObject* Owner, CLUAComponent* Component);
	void RemoveComponents();
	const std::vector<CScript *> & GetLUAScripts();
};

#endif