#include "GameObject\LuaGameObjectHandleManager.h"
#include "GameObject\GameObjectManager.h"
#include "Engine\Engine.h"
#include "Log\Log.h"

CLuaGameObjectHandleManager::CLuaGameObjectHandleManager()
{

}
CLuaGameObjectHandleManager::~CLuaGameObjectHandleManager()
{
}

CLuaGameObjectHandle CLuaGameObjectHandleManager::Add(CGameObject* GameObject)
{
	#ifdef _DEBUG
		if (GameObject == nullptr)
		{
			LOG("Can't create LuaGameObjectHandler with null GameObject");
			return NULL;
		}
	#endif
	m_LuaGameObjectHandles.push_back(CLuaGameObjectHandle(GameObject));
	return m_LuaGameObjectHandles[m_LuaGameObjectHandles.size() - 1];
}
CLuaGameObjectHandle CLuaGameObjectHandleManager::Get(const std::string &GameObjectName)
{
	for (size_t i = 0; i < m_LuaGameObjectHandles.size(); ++i)
	{
		if (m_LuaGameObjectHandles[i].GetName() == GameObjectName)
		{
			return m_LuaGameObjectHandles[i];
		}
	}
	#ifdef _DEBUG
		LOG("LuaGameObjectHandle "+GameObjectName + " doesn't exist");
	#endif
	return 0;
}
void CLuaGameObjectHandleManager::Remove(const std::string &GameObjectName)
{
	m_HandlesToBeDeleted.push_back(GameObjectName);
}

/*
Used to remove character collider some seconds before gameobject is removed
*/
void CLuaGameObjectHandleManager::RemoveCharacterCollider(const std::string &GameObjectName)
{
	CEngine::GetSingleton().GetGameObjectManager()->RemoveGameObjectCharacterCollider(GameObjectName);
}

void CLuaGameObjectHandleManager::RemoveHandle(const std::string &GameObjectName)
{
	for (size_t i = 0; i < m_LuaGameObjectHandles.size(); ++i)
	{
		if (m_LuaGameObjectHandles[i].GetName() == GameObjectName)
		{
			CEngine::GetSingleton().GetGameObjectManager()->RemoveGameObject(GameObjectName);
			m_LuaGameObjectHandles.erase(m_LuaGameObjectHandles.begin() + i);
			#ifdef _DEBUG
				LOG("Gameobject " + GameObjectName + " completely removed");
			#endif
		}
	}
}

void CLuaGameObjectHandleManager::Update()
{
	RemoveHandlesToBeDeleted();
}

void CLuaGameObjectHandleManager::RemoveHandlesToBeDeleted()
{
	for (size_t i = 0; i < m_HandlesToBeDeleted.size(); ++i)
	{
		RemoveHandle(m_HandlesToBeDeleted[i]);
	}
}

void CLuaGameObjectHandleManager::Destroy()
{
	m_LuaGameObjectHandles.clear();
}