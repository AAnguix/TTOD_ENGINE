#include "GameObject\LuaGameObjectHandle.h"
#include "Utils\GameObject.h"
#include "RenderableObjects\RenderableObject.h"
#include "Components\AnimatorController\AnimatorController.h"
#include "Components\Collider.h"
#include "Components\AudioSource.h"
#include "Log\Log.h"
#include "Engine\Engine.h"

CLuaGameObjectHandle::CLuaGameObjectHandle(CGameObject* GameObject)
{
	m_GameObject = GameObject;
}
CLuaGameObjectHandle::~CLuaGameObjectHandle()
{
}

CGameObject* CLuaGameObjectHandle::GetGameObject() const
{
	return m_GameObject;
}

const Vect3f & CLuaGameObjectHandle::GetPosition() const 
{
	CRenderableObject* l_RenderableObject = m_GameObject->GetRenderableObject();
	return l_RenderableObject->GetPosition();
}

Vect3f CLuaGameObjectHandle::GetForward() const{ return m_GameObject->GetRenderableObject()->GetForward(); }
Vect3f CLuaGameObjectHandle::GetUp() const{ return m_GameObject->GetRenderableObject()->GetUp(); }
Vect3f CLuaGameObjectHandle::GetRight() const{ return m_GameObject->GetRenderableObject()->GetRight(); }
float CLuaGameObjectHandle::GetYaw() const { return m_GameObject->GetRenderableObject()->GetYaw(); }
float CLuaGameObjectHandle::GetPitch() const { return m_GameObject->GetRenderableObject()->GetPitch(); }
float CLuaGameObjectHandle::GetRoll() const { return m_GameObject->GetRenderableObject()->GetRoll(); }
void CLuaGameObjectHandle::SetYaw(float Yaw){ m_GameObject->GetRenderableObject()->SetYaw(Yaw); }
void CLuaGameObjectHandle::SetPitch(float Pitch){ m_GameObject->GetRenderableObject()->SetPitch(Pitch); }
void CLuaGameObjectHandle::SetRoll(float Roll){ m_GameObject->GetRenderableObject()->SetRoll(Roll); }
void CLuaGameObjectHandle::SetYawPitchRoll(float Yaw, float Pitch, float Roll) { m_GameObject->GetRenderableObject()->SetYawPitchRoll(Yaw,Pitch,Roll); }

const std::string &CLuaGameObjectHandle::GetName()
{
	if (m_GameObject!=nullptr)
		return m_GameObject->GetName();
	
	#ifdef _DEBUG	
		LOG("LuaGameObjectHandler has null GameObject");
	#endif
	return "";
}

void CLuaGameObjectHandle::GameObjectError() const
{

}

void CLuaGameObjectHandle::AnimatorError() const
{
	#ifdef _DEBUG
		LOG("GameObject " + m_GameObject->GetName() + " doesn't have an AnimatorController");
	#endif
}
void CLuaGameObjectHandle::ColliderError() const
{
	#ifdef _DEBUG
		LOG("GameObject " + m_GameObject->GetName() + " doesn't have a Collider");
	#endif
}
void CLuaGameObjectHandle::AudioSourceError() const
{
	#ifdef _DEBUG
		LOG("GameObject " + m_GameObject->GetName() + " doesn't have an AudioSource");
	#endif
}

CState* CLuaGameObjectHandle::AddState(const std::string &Name, const std::string &Animation, float Speed, const std::string &OnEnter, const std::string &OnUpdate, const std::string &OnExit)
{
	CAnimatorController* l_Animator = m_GameObject->GetAnimatorController();
	if (l_Animator)	{ return l_Animator->AddState(Name, Animation, Speed, OnEnter, OnUpdate, OnExit);	}
	else { AnimatorError();	return nullptr; }
}
bool CLuaGameObjectHandle::AddInteger(const std::string &Name, int Value)
{
	CAnimatorController* l_Animator = m_GameObject->GetAnimatorController();
	if (l_Animator)	{ return l_Animator->AddInteger(Name, Value); } 
	else { AnimatorError(); return 0; }
}
bool CLuaGameObjectHandle::AddFloat(const std::string &Name, float Value)
{
	CAnimatorController* l_Animator = m_GameObject->GetAnimatorController();
	if (l_Animator) { return l_Animator->AddFloat(Name, Value);	}
	else { AnimatorError();	return .0f;	}
}
bool CLuaGameObjectHandle::AddBool(const std::string &Name, bool Value)
{
	CAnimatorController* l_Animator = m_GameObject->GetAnimatorController();
	if (l_Animator)	{ return l_Animator->AddBool(Name, Value); }
	else { AnimatorError();	return false; }
}
bool CLuaGameObjectHandle::AddTrigger(const std::string &Name, bool Value)
{
	CAnimatorController* l_Animator = m_GameObject->GetAnimatorController();
	if (l_Animator)	{ return l_Animator->AddTrigger(Name, Value); }
	else { AnimatorError(); return false; }
}
void CLuaGameObjectHandle::SetInteger(const std::string &Name, int Value)
{
	CAnimatorController* l_Animator = m_GameObject->GetAnimatorController();
	if (l_Animator)	{ l_Animator->SetInteger(Name, Value); }
	else { AnimatorError(); }
}
void CLuaGameObjectHandle::SetFloat(const std::string &Name, float Value)
{
	CAnimatorController* l_Animator = m_GameObject->GetAnimatorController();
	if (l_Animator)	{ l_Animator->SetFloat(Name, Value); }
	else { AnimatorError(); }
}
void CLuaGameObjectHandle::SetBool(const std::string &Name, bool Value)
{
	CAnimatorController* l_Animator = m_GameObject->GetAnimatorController();
	if (l_Animator)	{ l_Animator->SetBool(Name, Value); }
	else { AnimatorError(); }
}
void CLuaGameObjectHandle::SetTrigger(const std::string &Name)
{
	CAnimatorController* l_Animator = m_GameObject->GetAnimatorController();
	if (l_Animator)	{ l_Animator->SetTrigger(Name); }
	else { AnimatorError(); }
}

CMaterial* CLuaGameObjectHandle::GetPhysxMaterial() const
{
	CCollider* l_Collider= m_GameObject->GetCollider();
	if (l_Collider)
	{
		return l_Collider->GetPhysxMaterial();
	}
	else
	{
		ColliderError();
		return nullptr;
	}
}

bool CLuaGameObjectHandle::AddSound(const std::string &Key, const std::string &SoundEventName)
{
	CAudioSource* l_AudioSource = m_GameObject->GetAudioSource();
	if (l_AudioSource)	{ return l_AudioSource->AddSound(Key,SoundEventName); }
	else { AudioSourceError(); return false; }
}
void CLuaGameObjectHandle::PlayEvent(const std::string &Key)
{
	CAudioSource* l_AudioSource = m_GameObject->GetAudioSource();
	if (l_AudioSource)	{ l_AudioSource->PlayEvent(Key); }
	else { AudioSourceError(); }
}