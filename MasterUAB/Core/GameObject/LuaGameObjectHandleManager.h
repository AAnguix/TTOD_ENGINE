#ifndef _LUAGAMEOBJECTHANDLEMANAGER_H
#define _LUAGAMEOBJECTHANDLEMANAGER_H

#include <vector>
#include "GameObject\LuaGameObjectHandle.h"

class CGameObject;

class CLuaGameObjectHandleManager {

private:
	std::vector<CLuaGameObjectHandle> m_LuaGameObjectHandles;

public:
	CLuaGameObjectHandleManager();
	~CLuaGameObjectHandleManager();

	CLuaGameObjectHandle Add(CGameObject* GameObject);
	void Remove(const std::string &GameObjectName);
	CLuaGameObjectHandle Get(const std::string &GameObjectName);
	void Destroy();
};

#endif
