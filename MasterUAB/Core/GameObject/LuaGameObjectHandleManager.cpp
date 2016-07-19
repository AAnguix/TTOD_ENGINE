#include "GameObject\LuaGameObjectHandleManager.h"
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
	for (size_t i = 0; i < m_LuaGameObjectHandles.size(); ++i)
	{
		if (m_LuaGameObjectHandles[i].GetName() == GameObjectName)
		{
			m_LuaGameObjectHandles.erase(m_LuaGameObjectHandles.begin() + i);
		}
	}
}
void CLuaGameObjectHandleManager::Destroy()
{
	m_LuaGameObjectHandles.clear();
}