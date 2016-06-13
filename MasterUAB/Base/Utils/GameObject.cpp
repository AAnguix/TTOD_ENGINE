#include "Utils\GameObject.h"
#include "XML\XMLTreeNode.h"
#include "Components\Collider.h"
#include "Components\CharacterCollider.h"

CGameObject::CGameObject()
:CNamed("")
,m_AudioSource(nullptr)
,m_AnimatorController(nullptr)
,m_Script(nullptr)
,m_RenderableObject(nullptr)
,m_Collider(nullptr)
,m_CharacterCollider(nullptr)
{

}

CGameObject::CGameObject(const std::string &Name)
:CNamed(Name)
,m_AudioSource(nullptr)
,m_AnimatorController(nullptr)
,m_Script(nullptr)
,m_RenderableObject(nullptr)
,m_Collider(nullptr)
,m_CharacterCollider(nullptr)
{

}

CGameObject::CGameObject(const CXMLTreeNode &XMLTreeNode)
:CNamed(XMLTreeNode)
,m_AudioSource(nullptr)
,m_AnimatorController(nullptr)
,m_Script(nullptr)
,m_RenderableObject(nullptr)
,m_Collider(nullptr)
,m_CharacterCollider(nullptr)
{
}

CGameObject::~CGameObject()
{
}

CAnimatorController* CGameObject::GetAnimatorController() const { return m_AnimatorController; };
CScript* CGameObject::GetScript() const { return m_Script; };
CAudioSource* CGameObject::GetAudioSource() const { return m_AudioSource; };
CRenderableObject* CGameObject::GetRenderableObject() const{ return m_RenderableObject; };
CCollider* CGameObject::GetCollider() const { return m_Collider;}
CCharacterCollider* CGameObject::GetCharacterCollider() const { return m_CharacterCollider;}

CEmptyPointerClass* CGameObject::GetThisLuaAddress() const { return (CEmptyPointerClass *)(this); }

void CGameObject::SetAudioSource(CAudioSource* AudioSource)
{
	m_AudioSource = AudioSource;
}
void CGameObject::SetScript(CScript* Script)
{
	m_Script = Script;
}
void CGameObject::SetAnimatorController(CAnimatorController* AnimatorController)
{
	m_AnimatorController = AnimatorController;
}
void CGameObject::SetRenderableObject(CRenderableObject* RenderableObject)
{
	m_RenderableObject = RenderableObject;
}
void CGameObject::SetCollider(CCollider* CCollider)
{
	m_Collider = CCollider;
}
void CGameObject::SetCharacterCollider(CCharacterCollider* CharacterCollider)
{
	m_CharacterCollider = CharacterCollider;
}
