#include "Components\CharacterCollider.h"
#include "Components\ComponentManager.h"
#include "Engine.h"
#include "PhysXManager.h"
#include "Animation\AnimatedInstanceModel.h"
#include "RenderableObjects\RenderableObject.h"
#include "Cinematics\Cinematic.h"
#include "Materials\Material.h"

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
	
}
	
void CCharacterCollider::Render(CRenderManager &RenderManager)
{
	if (IsEnabled())
	{
		Vect3f l_PhysxPos = CEngine::GetSingleton().GetPhysXManager()->GetCharacterControllerFootPosition(m_Owner->GetName());
		m_Owner->SetPosition(l_PhysxPos);
	}
}

void CCharacterCollider::RenderDebug(CRenderManager &RenderManager)
{

}

CCharacterCollider* CCharacterCollider::AddCharacterCollider(const std::string &Name, CAnimatedInstanceModel *Owner)
{
	CCharacterCollider* l_CharacterCollider = new CCharacterCollider(Name, Owner);

	if (!Owner->AddComponent(l_CharacterCollider))
	{
		CHECKED_DELETE(l_CharacterCollider);
		return nullptr;
	}
	return l_CharacterCollider;
}