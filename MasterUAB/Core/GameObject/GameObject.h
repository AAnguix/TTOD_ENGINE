#ifndef _GAMEOBJECT_H
#define _GAMEOBJECT_H

#include "Utils\3DElement.h"
#include "Utils\Named.h"

class CAnimatorController;
class CScript;
class CAudioSource;
class CEmptyPointerClass;
class CRenderableObject;
class CCollider;
class CCharacterCollider;

class CGameObject : public CNamed
{

protected:
	CAnimatorController* m_AnimatorController;
	CScript* m_Script;
	CAudioSource* m_AudioSource;
	CRenderableObject* m_RenderableObject;
	CCollider *m_Collider;
	CCharacterCollider* m_CharacterCollider;

public:
	CGameObject();
	CGameObject(const std::string &Name);
	CGameObject(const CXMLTreeNode &XMLTreeNode);
	virtual ~CGameObject();

	virtual CAnimatorController* GetAnimatorController() const;
	virtual CScript* GetScript() const;
	virtual CAudioSource* GetAudioSource() const;
	virtual CRenderableObject* GetRenderableObject() const;
	virtual CCollider* GetCollider() const;
	virtual CCharacterCollider* GetCharacterCollider() const;

	virtual void SetAudioSource(CAudioSource* AudioSource);
	virtual void SetScript(CScript* Script);
	virtual void SetAnimatorController(CAnimatorController* AnimatorController);
	virtual void SetRenderableObject(CRenderableObject* RenderableObject);
	virtual void SetCollider(CCollider* CCollider);
	virtual void SetCharacterCollider(CCharacterCollider* CharacterCollider);

	virtual CEmptyPointerClass* GetThisLuaAddress() const;
};

#endif