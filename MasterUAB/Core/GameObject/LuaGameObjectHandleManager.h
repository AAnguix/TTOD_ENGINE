#ifndef _LUAGAMEOBJECTHANDLEMANAGER_H
#define _LUAGAMEOBJECTHANDLEMANAGER_H

#include <vector>
#include "GameObject\LuaGameObjectHandle.h"

class CGameObject;

class CLuaGameObjectHandleManager {

private:
	/*Handlers will be removed the next frame*/
	std::vector<const std::string> m_HandlesToBeDeleted;
	std::vector<CLuaGameObjectHandle> m_LuaGameObjectHandles;

	void RemoveHandlesToBeDeleted();
	void RemoveHandle(const std::string &GameObjectName);

public:
	CLuaGameObjectHandleManager();
	~CLuaGameObjectHandleManager();

	void Update();
	CLuaGameObjectHandle Add(CGameObject* GameObject);
	void Remove(const std::string &GameObjectName);
	void RemoveCharacterCollider(const std::string &GameObjectName);

	CLuaGameObjectHandle Get(const std::string &GameObjectName);
	void Destroy();
};

#endif
