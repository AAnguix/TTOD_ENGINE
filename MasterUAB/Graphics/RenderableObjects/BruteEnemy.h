#ifndef _BRUTEENEMY_H
#define _BRUTEENEMY_H

#include "Enemy.h"

class CBruteEnemy : public CEnemy
{

private:
	float m_ChargeForce;
	bool Charge(Vect3f Position, float ElapsedTime);

public:
	CBruteEnemy(float Health, float Speed, float AttackDelay, float VisionRange, std::vector<Vect3f> WayPoints, float ChargeForce);
	void Attack(float ElapsedTime);

};

#endif