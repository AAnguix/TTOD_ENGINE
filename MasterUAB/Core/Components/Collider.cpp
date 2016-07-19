#include "Components\Collider.h"
#include "GameObject\GameObject.h"
#include "RenderableObjects\MeshInstance.h"
#include "StaticMeshes\StaticMesh.h"
#include "Engine\Engine.h"
#include "PhysXManager.h"
#include "Materials\Material.h"

CCollider::CCollider(const std::string &Name, CGameObject *Owner)
:CComponent(Name,Owner)
{
	m_PhysxMaterial = ((CMeshInstance*)Owner->GetRenderableObject())->GetStaticMesh()->GetPhysxMaterial();
}

CCollider::~CCollider()
{
}

/*Update graphics transform from physix transform*/
void CCollider::Update(float ElapsedTime)
{
	if (IsEnabled())
	{
		assert(m_Owner->GetRenderableObject() != nullptr);
		CPhysXManager::SActorData l_Data = CEngine::GetSingleton().GetPhysXManager()->GetActorPositionAndOrientation(m_Owner->GetName());
		m_Owner->GetRenderableObject()->SetPosition(l_Data.m_Position);
		Vect3f l_V = l_Data.m_Orientation.GetRadians();
		m_Owner->GetRenderableObject()->SetYawPitchRoll(l_V.x, l_V.y, l_V.z);
	}
}

CMaterial* CCollider::GetPhysxMaterial() const
{
	return m_PhysxMaterial; 
}


