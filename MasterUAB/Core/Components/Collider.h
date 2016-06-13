#ifndef _COLLIDER_H
#define _COLLIDER_H

#include "Components\Component.h"
class CGameObject;
class CMaterial;

class CCollider : public CComponent
{
private:
	CMaterial* m_PhysxMaterial;

public:
	CCollider(const std::string &Name, CGameObject *Owner);
	virtual ~CCollider();
	virtual void Update(float ElapsedTime);

	CMaterial* GetPhysxMaterial() const;
};

#endif