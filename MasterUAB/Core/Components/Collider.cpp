#include "Components\Collider.h"
#include "Components\ComponentManager.h"
#include "Engine.h"
#include "PhysXManager.h"
#include "RenderableObjects\RenderableObject.h"

CCollider::CCollider(const std::string &Name, CRenderableObject *Owner)
:CComponent(Name,Owner)
{

}

CCollider::~CCollider()
{
	
}

void CCollider::Update(float ElapsedTime)
{
	
}
	
void CCollider::Render(CRenderManager &RenderManager)
{
	if (IsEnabled())
	{
		CPhysXManager::SActorData l_Data = CEngine::GetSingleton().GetPhysXManager()->GetActorPositionAndOrientation(m_Owner->GetName());
		m_Owner->SetPosition(l_Data.m_Position);
		Vect3f l_V = l_Data.m_Orientation.GetRadians();
		m_Owner->SetYaw(l_V.x);
		m_Owner->SetPitch(l_V.y);
		m_Owner->SetRoll(l_V.z);
	}
}

void CCollider::RenderDebug(CRenderManager &RenderManager)
{

}

CCollider* CCollider::AddCollider(const std::string &Name, CRenderableObject *Owner)
{
	CCollider* l_Collider = new CCollider(Name, Owner);

	if (!Owner->AddComponent(l_Collider))
	{
		CHECKED_DELETE(l_Collider);
	}
	else
	{
		assert(Owner->GetClassType() == CRenderableObject::MESH_INSTANCE);
	}

	return l_Collider;
}