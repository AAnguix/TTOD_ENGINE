#include "Components\Collider.h"
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

/*Update graphics transform from physix transform*/
void CCollider::Update(float ElapsedTime)
{
	if (IsEnabled())
	{
		CPhysXManager::SActorData l_Data = CEngine::GetSingleton().GetPhysXManager()->GetActorPositionAndOrientation(m_Owner->GetName());
		m_Owner->SetPosition(l_Data.m_Position);
		Vect3f l_V = l_Data.m_Orientation.GetRadians();
		m_Owner->SetYawPitchRoll(l_V.x, l_V.y, l_V.z);
	}
}

CMaterial* CCollider::GetPhysxMaterial() const
{
	return m_PhysxMaterial; 
}


