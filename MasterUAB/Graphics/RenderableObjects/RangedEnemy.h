#ifndef _RANGEDNEMY_H
#define _RANGEDNEMY_H

#include "Enemy.h"

class CRangedEnemy : public CEnemy
{

private:
	float m_ShootingForce;
	Vect3f m_Target;

public:
	CRangedEnemy(float Health, float Speed, float AttackDelay, float VisionRange, std::vector<Vect3f> WayPoints, float ShootingForce);
	
	void SetTarget(Vect3f Position);
	void Attack(float ElapsedTime);

};

#endif