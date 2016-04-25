#include "Enemy.h"
#include "Armor.h"
#include "Weapon.h"

CEnemy::CEnemy(float Health, float Speed, float AttackDelay, float VisionRange, std::vector<Vect3f> WayPoints)
:CAnimatedInstanceModel("")
,m_Health(Health), m_Speed(Speed), m_AttackDelay(AttackDelay)
,m_VisionRange(VisionRange),m_WayPoints(WayPoints)
,m_CurrentWayPoint(0)
{
	//m_Player = CEngine::GetSingleton().GetLayerManager()->GetPlayer()
}

CEnemy::~CEnemy()
{
	CHECKED_DELETE(m_Armor);
	CHECKED_DELETE(m_Weapon);
	m_VoicesSounds.clear();
	m_WayPoints.clear();
}

void CEnemy::TakeDamage(float Damage)
{
	//TODO Get damage reduction pct
	float l_DamageReductionPct = 0.0f;
	float l_Damage = Damage - Damage*l_DamageReductionPct;

	m_Health -= l_Damage;

	if (m_Health <= 0.0f)
	{
		m_Dead = true;
		Kill();
	}
}

void Kill()
{
	//Reproducir sonido y animación de muerte
}

bool CEnemy::IsPlayerInsideVisionRange(Vect3f PlayerPosition)
{
	float l_Distance = 0.0f;
	bool l_IsPlayerInsideVisionRange = false;
	
	l_Distance = sqrt(pow(m_Position.x - PlayerPosition.x, 2) + pow(m_Position.y - PlayerPosition.y, 2) + pow(m_Position.z - PlayerPosition.z, 2));

	if (l_Distance < m_VisionRange)
		l_IsPlayerInsideVisionRange = true;

	return l_IsPlayerInsideVisionRange;
}

void CEnemy::FollowTriangleWayPoints(float ElapsedTime, const std::vector<int> Indexes)
{
	if (Indexes.size()>1)
	{
		Vect3f l_Vector = m_WayPoints[Indexes[m_CurrentWayPoint]] - m_Position;
		l_Vector.Normalize();

		if (!PointInsideCircle(m_Position, m_WayPoints[Indexes[m_CurrentWayPoint]], POINT_RADIUS))
		{
			SetPosition(m_Position + ((l_Vector*ElapsedTime*m_Speed)));
		}
		else
		{
			m_CurrentWayPoint++;
		}
		
		if (m_CurrentWayPoint >= Indexes.size())
		{
			m_CurrentWayPoint = 0;
		}
	}
}

bool CEnemy::PointInsideCircle(Vect3f OriginPoint, Vect3f  DestinyPoint, float Radius)
{
	Vect3f l_Vector = DestinyPoint - OriginPoint;
	float l_Distance = l_Vector.Length();

	if (l_Distance < Radius)
		return true;

	return false;	
}