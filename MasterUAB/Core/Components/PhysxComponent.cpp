#include "Components\PhysxComponent.h"
#include "Components\ComponentManager.h"
#include "Engine.h"
#include "PhysXManager.h"
#include "Animation\AnimatedInstanceModel.h"
#include "RenderableObjects\RenderableObject.h"
#include "Cinematics\Cinematic.h"

CPhysxComponent::CPhysxComponent(const std::string &Name, CRenderableObject *Owner, bool CharacterController)
	: CComponent(Name,Owner)
{
	m_CharacterController=CharacterController;
}

CPhysxComponent::~CPhysxComponent()
{
	
}

void CPhysxComponent::Update(float ElapsedTime)
{
	
}
	
void CPhysxComponent::Render(CRenderManager &RenderManager)
{
	if (m_Owner->GetClassType() == CRenderableObject::ANIMATED_INSTANCE)
	{
		Vect3f l_PhysxPos = CEngine::GetSingleton().GetPhysXManager()->GetCharacterControllerFootPosition(m_Owner->GetName());
		m_Owner->SetPosition(Vect3f(l_PhysxPos.x, l_PhysxPos.y, l_PhysxPos.z));
	}
	else 
	{
		CPhysXManager::SActorData l_Data = CEngine::GetSingleton().GetPhysXManager()->GetActorPositionAndOrientation(m_Owner->GetName());
		m_Owner->SetPosition(l_Data.m_Position);
		Vect3f l_V = l_Data.m_Orientation.GetRadians();
		m_Owner->SetYaw(l_V.x);
		m_Owner->SetPitch(l_V.y);
		m_Owner->SetRoll(l_V.z);
	}
}

void CPhysxComponent::RenderDebug(CRenderManager &RenderManager)
{

}

CPhysxComponent* CPhysxComponent::CreatePhysxComponent(const std::string &Name, CRenderableObject *Owner, bool CharacterController)
{
	CPhysxComponent* l_PhysxComponent = new CPhysxComponent(Name,Owner, CharacterController);

	if(!Owner->AddComponent(l_PhysxComponent))
	{
		CHECKED_DELETE(l_PhysxComponent);
	}

	return l_PhysxComponent;
}