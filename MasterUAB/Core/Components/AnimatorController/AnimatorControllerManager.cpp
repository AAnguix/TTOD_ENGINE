#include "AnimatorControllerManager.h"
#include "Components\AnimatorController\AnimatorController.h"
#include <assert.h>
#include "GameObject\GameObject.h"
#include "GameObject\LuaGameObjectHandle.h"
#include "Engine\Engine.h"
#include "Log\Log.h"

CAnimatorControllerManager::CAnimatorControllerManager()
{
}
CAnimatorControllerManager::~CAnimatorControllerManager()
{
	RemoveComponents();
}

void CAnimatorControllerManager::UpdateComponents(float ElapsedTime)
{
	for (size_t i = 0; i < m_Components.size(); ++i)
	{
		m_Components[i]->Update(ElapsedTime);
	}
}

void CAnimatorControllerManager::RemoveComponent(CGameObject* Owner)
{
	for (size_t i = 0; i < m_Components.size(); ++i)
	{
		if (m_Components[i]->GetOwner() == Owner)
		{
			delete m_Components[i];
			m_Components.erase(m_Components.begin() + i);
		}
	}
}

CAnimatorController* CAnimatorControllerManager::AddComponent(const std::string &Name, CLuaGameObjectHandle* Owner)
{
	bool l_Found = false;
	CAnimatorController* l_AnimatorController = nullptr;
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
		l_AnimatorController = new CAnimatorController(Name, l_GObject);
		l_GObject->SetAnimatorController(l_AnimatorController);
		m_Components.push_back(l_AnimatorController);
	}
	else
	{
		assert(false);
		delete(l_AnimatorController); l_AnimatorController = NULL;
	}

	return l_AnimatorController;
}
void CAnimatorControllerManager::RemoveComponents()
{
	for (size_t i = 0; i < m_Components.size(); ++i)
	{
		delete m_Components[i];
		m_Components[i] = NULL;
	}
	m_Components.clear();
}
void CAnimatorControllerManager::Update(float ElapsedTime)
{
	UpdateComponents(ElapsedTime);
}