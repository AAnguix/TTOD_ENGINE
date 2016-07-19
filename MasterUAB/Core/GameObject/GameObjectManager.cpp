#include "GameObject\GameObjectManager.h"
#include "GameObject\GameObject.h"

CGameObjectManager::CGameObjectManager()
{
}

CGameObjectManager::~CGameObjectManager()
{
}

void CGameObjectManager::SetPlayer(CGameObject* Player)
{
	m_Player = Player;
}

CGameObject* CGameObjectManager::AddGameObject(CXMLTreeNode &TreeNode)
{
	CGameObject* l_GameObject = new CGameObject(TreeNode);

	if (!AddResource(l_GameObject->GetName(), l_GameObject))
	{
		delete(l_GameObject);
		l_GameObject = NULL;
	}
	return l_GameObject;
}

CGameObject* CGameObjectManager::AddGameObject(const std::string &GameObjectName)
{
	CGameObject* l_GameObject = new CGameObject(GameObjectName);

	if (!AddResource(l_GameObject->GetName(), l_GameObject))
	{
		delete(l_GameObject);
		l_GameObject = NULL;
	}
	return l_GameObject;
}