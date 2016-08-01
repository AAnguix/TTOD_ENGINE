#include "Components\CharacterCollider.h"
#include "GameObject\GameObject.h"
#include "Engine\Engine.h"
#include "PhysXManager.h"
#include "Animation\AnimatedInstanceModel.h"
#include "Animation\AnimatedCoreModel.h"

CCharacterCollider::CCharacterCollider(const std::string &Name, CGameObject *Owner)
:CComponent(Name,Owner)
,m_PhysxMaterial(false)
{
	CAnimatedInstanceModel* l_AnimatedInstance = ((CAnimatedInstanceModel*)Owner->GetRenderableObject());
	CAnimatedCoreModel* l_CoreModel = l_AnimatedInstance->GetAnimatedCoreModel();
	assert(l_CoreModel != nullptr);
	assert(l_CoreModel->GetMaterials().size() > 0);
	m_PhysxMaterial = l_CoreModel->GetMaterials()[0];
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