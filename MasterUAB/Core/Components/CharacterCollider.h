#ifndef _CHARACTERCOLLIDER_H
#define _CHARACTERCOLLIDER_H

#include "Components\Component.h"
class CAnimatedInstanceModel;
class CMaterial;

class CCharacterCollider : public CComponent
{
private:
	CMaterial* m_PhysxMaterial;

public:
	CCharacterCollider(const std::string &Name, CAnimatedInstanceModel *Owner);
	virtual ~CCharacterCollider();
	virtual void Update(float ElapsedTime);
	virtual void Render(CRenderManager &RenderManager);
	virtual void RenderDebug(CRenderManager &RenderManager);
	static CCharacterCollider * AddCharacterCollider(const std::string &Name, CAnimatedInstanceModel *Owner);
	
	CMaterial* GetPhysxMaterial() const { return m_PhysxMaterial; };
};

#endif