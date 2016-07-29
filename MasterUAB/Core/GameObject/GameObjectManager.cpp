#include "GameObject\GameObjectManager.h"
#include "GameObject\GameObject.h"
#include "Components\AnimatorController\AnimatorControllerManager.h"
#include "PhysXManager.h"
#include "ISoundManager.h"
#include "Components\Script\ScriptManager.h"
#include "RenderableObjects\LayerManager.h"
#include "Engine\Engine.h"
#include "Log\Log.h"

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

void CGameObjectManager::RemoveGameObject(const std::string &Name)
{
	CGameObject* l_GameObject = GetResource(Name);
	if (l_GameObject)
	{
		if (l_GameObject->GetAnimatorController()) { CEngine::GetSingleton().GetAnimatorControllerManager()->RemoveComponent(l_GameObject); l_GameObject->SetAnimatorController(nullptr); }
		if (l_GameObject->GetRenderableObject()) { CEngine::GetSingleton().GetLayerManager()->RemoveRenderableObject(l_GameObject->GetName()); }
		if (l_GameObject->GetCollider()) { CEngine::GetSingleton().GetPhysXManager()->RemoveColliderComponent(l_GameObject); l_GameObject->SetCollider(nullptr); }
		if (l_GameObject->GetCharacterCollider()) { CEngine::GetSingleton().GetPhysXManager()->RemoveCharacterColliderComponent(l_GameObject); l_GameObject->SetCharacterCollider(nullptr); }
		if (l_GameObject->GetAudioSource()) { CEngine::GetSingleton().GetSoundManager()->RemoveComponent(l_GameObject); l_GameObject->SetAudioSource(nullptr); }
		if (l_GameObject->GetScript()) { CEngine::GetSingleton().GetScriptManager()->RemoveComponent(l_GameObject); l_GameObject->SetScript(nullptr); }
		RemoveResource(Name);
	}
	else
	{
		#ifdef _DEBUG
			LOG("Trying to remove a gameobject that doesn't exists");
		#endif
	}
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