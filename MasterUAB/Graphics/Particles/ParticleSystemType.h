#ifndef _PARTICLESYSTEMTYPE_H
#define _PARTICLESYSTEMTYPE_H

class CXMLTreeNode;
#include "Math\Vector2.h"
#include "Math\Color.h"
#include "Utils\Named.h"
#include "Materials\Material.h"

class CParticleSystemType : public CNamed
{

private:
	CMaterial* m_Material;

public:
	struct ControlPointColor
	{
		Vect2f m_Time;
		CColor m_Color1, m_Color2;
		ControlPointColor(CColor Color1, CColor Color2, Vect2f Time) :m_Color1(Color1), m_Color2(Color2), m_Time(Time){};
	};
	struct ControlPointSize
	{
		Vect2f m_Time;
		Vect2f m_Size;
		ControlPointSize(Vect2f Size, Vect2f Time) : m_Size(Size), m_Time(Time){};
	};

	std::vector<ControlPointColor> m_ControlPointColor;
	std::vector<ControlPointSize> m_ControlPointSizes;

	int m_NumFrames;
	float m_TimerPerFrame;
	bool m_LoopFrames;

	bool m_EmitAbsolute;
	float m_StartingDirectionAngle, m_StartingAccelerationAngle;
	Vect2f m_Size;
	Vect2f m_EmitRate, m_AwakeTime, m_SleepTime, m_Life;
	Vect2f m_StartingAngle, m_StartingAngularSpeed, m_AngularAcceleration;
	Vect3f m_StartingSpeed1, m_StartingSpeed2;
	Vect3f m_StartingAcceleration1, m_StartingAcceleration2;
	CColor m_Color1, m_Color2;

	CParticleSystemType(CXMLTreeNode &TreeNode);
	virtual ~CParticleSystemType();
	UAB_GET_PROPERTY_POINTER(CMaterial, Material);

};

#endif