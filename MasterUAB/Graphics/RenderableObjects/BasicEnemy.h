#ifndef _BASICENEMY_H
#define _BASICENEMY_H

#include "Enemy.h"

class CBasicEnemy : public CEnemy
{
private:
	float m_AttackRange;
	bool m_PlayerInsideAttackRange;

public:
	CBasicEnemy(std::vector<Vect3f> WayPoints);
	CBasicEnemy(float Health, float Speed, float AttackDelay, float VisionRange, std::vector<Vect3f> WayPoints, float AttackRange);
	void Attack();
	void MoveToPlayerNearestPoint(Vect3f Destination);

};

#endif