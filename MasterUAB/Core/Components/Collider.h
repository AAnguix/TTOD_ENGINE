#ifndef _COLLIDER_H
#define _COLLIDER_H

#include "Components\Component.h"

class CCollider : public CComponent
{

public:
	CCollider(const std::string &Name, CRenderableObject *Owner);
	virtual ~CCollider();
	virtual void Update(float ElapsedTime);
	virtual void Render(CRenderManager &RenderManager);
	virtual void RenderDebug(CRenderManager &RenderManager);
	static CCollider * AddCollider(const std::string &Name, CRenderableObject *Owner);
};

#endif