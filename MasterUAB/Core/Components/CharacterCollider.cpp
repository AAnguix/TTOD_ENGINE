#include "Components\CharacterCollider.h"
#include "Utils\GameObject.h"
#include "Engine\Engine.h"
#include "PhysXManager.h"
#include "Animation\AnimatedInstanceModel.h"
#include "Animation\AnimatedCoreModel.h"

CCharacterCollider::CCharacterCollider(const std::string &Name, CGameObject *Owner)
:CComponent(Name,Owner)
{
	m_PhysxMaterial = ((CAnimatedInstanceModel*)Owner->GetRenderableObject())->GetAnimatedCoreModel()->GetMaterials()[0];
}

CCharacterCollider::~CCharacterCollider()
{
}

void CCharacterCollider::Update(float ElapsedTime)
{
	if (IsEnabled())
	{
		assert(m_Owner->GetRenderableObject() != nullptr);
		Vect3f l_PhysxPos = CEngine::GetSingleton().GetPhysXManager()->GetCharacterControllerFootPosition(m_Owner->GetName());
		m_Owner->GetRenderableObject()->SetPosition(l_PhysxPos);
	}
}