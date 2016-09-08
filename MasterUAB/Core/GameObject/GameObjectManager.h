#ifndef _GAMEOBJECTMANAGER_H
#define _GAMEOBJECTMANAGER_H

#include "Utils\TemplatedVectorMapManager.h"
#include "GameObject\GameObject.h"
class CGameObject;
#include <assert.h>

class CGameObjectManager : public CTemplatedVectorMapManager<CGameObject>
{
private:
	CGameObject* m_Player;

public:
	CGameObjectManager();
	~CGameObjectManager();

	void SetPlayer(CGameObject* Player);
	CGameObject* GetPlayer() const{ assert(m_Player != nullptr); return m_Player; };

	void RemoveGameObject(const std::string &Name);
	void RemoveGameObjectCharacterCollider(const std::string &Name);

	CGameObject* AddGameObject(CXMLTreeNode &TreeNode);
	CGameObject* AddGameObject(const std::string &GameObjectName);
};

#endif