#include "Components\CharacterCollider.h"
#include "Engine.h"
#include "PhysXManager.h"
#include "Animation\AnimatedInstanceModel.h"
#include "Animation\AnimatedCoreModel.h"

CCharacterCollider::CCharacterCollider(const std::string &Name, CAnimatedInstanceModel *Owner)
:CComponent(Name,Owner)
,m_PhysxMaterial(Owner->GetAnimatedCoreModel()->GetMaterials()[0])
{
}

CCharacterCollider::~CCharacterCollider()
{
}

void CCharacterCollider::Update(float ElapsedTime)
{
	if (IsEnabled())
	{
		Vect3f l_PhysxPos = CEngine::GetSingleton().GetPhysXManager()->GetCharacterControllerFootPosition(m_Owner->GetName());
		m_Owner->SetPosition(l_PhysxPos);
	}
}