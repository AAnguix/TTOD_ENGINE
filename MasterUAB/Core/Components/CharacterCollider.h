#ifndef _CHARACTERCOLLIDER_H
#define _CHARACTERCOLLIDER_H

#include "Components\Component.h"
class CGameObject;
class CMaterial;

class CCharacterCollider : public CComponent
{
private:
	CMaterial* m_PhysxMaterial;

public:
	CCharacterCollider(const std::string &Name, CGameObject* Owner);
	virtual ~CCharacterCollider();
	virtual void Update(float ElapsedTime);
	
	CMaterial* GetPhysxMaterial() const { return m_PhysxMaterial; };
};

#endif