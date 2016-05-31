#include "Components\Collider.h"
#include "Components\ComponentManager.h"
#include "Engine.h"
#include "PhysXManager.h"
#include "RenderableObjects\MeshInstance.h"
#include "StaticMeshes\StaticMesh.h"
#include "Materials\Material.h"

CCollider::CCollider(const std::string &Name, CMeshInstance *Owner)
:CComponent(Name,Owner)
,m_PhysxMaterial(Owner->GetStaticMesh()->GetPhysxMaterial())
{
}

CCollider::~CCollider()
{
}

void CCollider::Update(float ElapsedTime)
{
}

/*Update graphics transform from physix transform*/
void CCollider::Render(CRenderManager &RenderManager)
{
	if (IsEnabled())
	{
		CPhysXManager::SActorData l_Data = CEngine::GetSingleton().GetPhysXManager()->GetActorPositionAndOrientation(m_Owner->GetName());
		m_Owner->SetPosition(l_Data.m_Position);
		Vect3f l_V = l_Data.m_Orientation.GetRadians();
		m_Owner->SetYawPitchRoll(l_V.x, l_V.y, l_V.z);
	}
}

void CCollider::RenderDebug(CRenderManager &RenderManager)
{
}

CCollider* CCollider::AddCollider(const std::string &Name, CMeshInstance *Owner)
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

CMaterial* CCollider::GetPhysxMaterial() const
{
	return m_PhysxMaterial; 
}


