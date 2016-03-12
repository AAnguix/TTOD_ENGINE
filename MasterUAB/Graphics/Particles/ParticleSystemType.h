#ifndef _PARTICLESYSTEMTYPE_H
#define _PARTICLESYSTEMTYPE_H

class CMaterial;
class CXMLTreeNode;
#include "Math\Vector2.h"
#include "Math\Color.h"
#include "Utils\Named.h"

class CParticleSystemType : public CNamed
{

private:
	CMaterial* m_Material;

	int m_NumFrames;
	float m_TimerPerFrame;
	bool m_LoopFrames;

	float m_StartingDirectionAngle, m_StartingAccelerationAngle;
	Vect2f m_Size;
	Vect2f m_EmitRate, m_AwakeTime, m_SleepTime, m_Life;
	Vect2f m_StartingAngle, m_StartingAngularSpeed, m_AngularAcceleration;
	Vect2f m_StartingSpeed1, m_StartingSpeed2;
	Vect2f m_StartingAcceleration1, m_StartingAcceleration2;
	CColor m_Color1, m_Color2;

public:
	bool m_EmitAbsolute;
	CParticleSystemType(CXMLTreeNode &TreeNode);
	CParticleSystemType();
	virtual ~CParticleSystemType();

};

#endif