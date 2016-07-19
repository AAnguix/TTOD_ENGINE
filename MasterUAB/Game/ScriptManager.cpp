#include "ScriptManager.h"
#include "GameObject\LuaGameObjectHandle.h"
#include "GameObject\GameObject.h"
#include "Components\LuaComponent.h"
#include "Components\Script.h"
#include <assert.h>

CScriptManager::CScriptManager()
{
}
CScriptManager::~CScriptManager()
{
	RemoveComponents();
}

void CScriptManager::Update(float ElapsedTime)
{
	UpdateComponents(ElapsedTime);
}

void CScriptManager::UpdateComponents(float ElapsedTime)
{
	for (size_t i = 0; i < m_Components.size(); ++i)
	{
		if (m_Components[i])
			m_Components[i]->Update(ElapsedTime);
	}
}

CScript* CScriptManager::AddComponent(const std::string& Name, CLuaGameObjectHandle* Owner, CLUAComponent* Component)
{
	bool l_Found = false;
	CScript* l_Script = nullptr;
	for (size_t i = 0; i < m_Components.size(); ++i)
	{
		if (m_Components[i]->GetName() == Name)
		{
			l_Found = true;
			i = m_Components.size();
		}
	}
	if (!l_Found)
	{
		CGameObject* l_GObject = Owner->GetGameObject();
		l_Script = new CScript(Name, l_GObject, Component);
		l_GObject->SetScript(l_Script);
		m_Components.push_back(l_Script);
	}
	else
	{
		assert(false);
		delete(l_Script); l_Script = NULL;
	}

	return l_Script;
}

void CScriptManager::RemoveComponents()
{
	for (size_t i = 0; i < m_Components.size(); ++i)
	{
		delete m_Components[i];
		m_Components[i] = NULL;
	}
	m_Components.clear();
}