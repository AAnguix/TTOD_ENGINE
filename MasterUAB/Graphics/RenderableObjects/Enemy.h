#ifndef _ENEMY_H
#define _ENEMY_H

#include "Animation\AnimatedInstanceModel.h"

const float POINT_RADIUS = 0.3f;

class CWeapon;
class CArmor;

class CEnemy : public CAnimatedInstanceModel
{
private:
	bool m_Dead;
	void Kill();

protected:
	float m_Health;
	CArmor* m_Armor;
	CWeapon* m_Weapon;
	//CRenderableObject* m_Player;

	std::vector<std::string> m_VoicesSounds;
	std::string m_PlayerDetectedSound;
	std::string m_DeadSound;
	std::string m_GoingToAttackSound;
	std::string m_DamageTakenSound;
	std::vector<Vect3f> m_WayPoints;
	unsigned int m_CurrentWayPoint;

	float m_Speed;
	float m_AttackDelay;
	float m_VisionRange;

	bool PointInsideCircle(Vect3f OriginPoint, Vect3f  DestinyPoint, float Radius);

public:
	CEnemy(float Health, float Speed, float AttackDelay, float VisionRange, std::vector<Vect3f> WayPoints);
	virtual ~CEnemy();

	CArmor* GetArmor(){ return m_Armor; };
	CWeapon* GetWeapon(){ return m_Weapon; };
	void TakeDamage(float Damage);
	bool IsPlayerInsideVisionRange(Vect3f PlayerPosition);

	virtual void Attack(float ElapsedTime) = 0;
	virtual void FollowTriangleWayPoints(float ElapsedTime, const std::vector<int> Indexes);
	
};

#endif
