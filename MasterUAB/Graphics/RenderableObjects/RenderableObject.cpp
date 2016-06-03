#include "RenderableObject.h"
#include <assert.h>

CRenderableObject::CRenderableObject(const CXMLTreeNode &XMLTreeNode) 
:CNamed(XMLTreeNode)
,C3DElement(XMLTreeNode)
,m_AudioSource(nullptr)
,m_AnimatorController(nullptr)
,m_Script(nullptr)
{
	
}

CRenderableObject::CRenderableObject(const std::string &Name, const Vect3f &Position, float Yaw, float Pitch, float Roll)
:CNamed(Name)
,C3DElement(Position,Yaw,Pitch,Roll)
{

}

CRenderableObject::~CRenderableObject()
{

}

void CRenderableObject::Update(float ElapsedTime)
{
	
}

CEmptyPointerClass* CRenderableObject::GetThisLuaAddress() const { return (CEmptyPointerClass *)(this); }

CAnimatorController* CRenderableObject::GetAnimatorController() const { return m_AnimatorController; };
CScript* CRenderableObject::GetScript() const { return m_Script; };
CAudioSource* CRenderableObject::GetAudioSource() const { return m_AudioSource; };
void CRenderableObject::SetAudioSource(CAudioSource* AudioSource)
{
	m_AudioSource = AudioSource;
}
void CRenderableObject::SetScript(CScript* Script)
{
	m_Script = Script;
}
void CRenderableObject::SetAnimatorController(CAnimatorController* AnimatorController)
{
	m_AnimatorController = AnimatorController;
}
	