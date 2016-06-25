#ifndef _PARTICLESYSTEMTYPE_H
#define _PARTICLESYSTEMTYPE_H


#include "Math\Vector2.h"
#include "Math\Color.h"
#include "Utils\Named.h"
#include <vector>

class CXMLTreeNode;
class CEmptyPointerClass;
class CMaterial;

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
		//CEmptyPointerClass* GetThisLuaAddress() const;
	};
	struct ControlPointSize
	{
		Vect2f m_Time;
		Vect2f m_Size;
		ControlPointSize(Vect2f Size, Vect2f Time) : m_Size(Size), m_Time(Time){};
		//CEmptyPointerClass* GetThisLuaAddress() const;
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
	CMaterial* GetMaterial() const { return m_Material; }

	virtual CEmptyPointerClass* GetNumFramesLuaAddress() const;
	virtual CEmptyPointerClass* GetTimePerFrameLuaAddress() const;
	virtual CEmptyPointerClass* GetLoopLuaAddress() const;
	virtual CEmptyPointerClass* GetEmitAbsoluteLuaAddress() const;

	virtual CEmptyPointerClass* GetStartingDirectionAngleLuaAddress() const;
	virtual CEmptyPointerClass* GetStartingAccelerationAngleLuaAddress() const;
	virtual CEmptyPointerClass* GetSizeLuaAddress(int Index);

	virtual CEmptyPointerClass* GetEmitRateLuaAddress(int Index);
	virtual CEmptyPointerClass* GetAwakeTimeLuaAddress(int Index);
	virtual CEmptyPointerClass* GetSleepTimeLuaAddress(int Index);
	virtual CEmptyPointerClass* GetLifeLuaAddress(int Index);

	virtual CEmptyPointerClass* GetStartingAngleLuaAddress(int Index);
	virtual CEmptyPointerClass* GetStartingAngularSpeedLuaAddress(int Index);
	virtual CEmptyPointerClass* GetAngularAccelerationLuaAddress(int Index);

	virtual CEmptyPointerClass* GetStartingSpeed1LuaAddress(int Index);
	virtual CEmptyPointerClass* GetStartingSpeed2LuaAddress(int Index);
	virtual CEmptyPointerClass* GetStartingAcceleration1LuaAddress(int Index);
	virtual CEmptyPointerClass* GetStartingAcceleration2LuaAddress(int Index);
	virtual CEmptyPointerClass* GetColor1LuaAddress() const;
	virtual CEmptyPointerClass* GetColor2LuaAddress() const;

	virtual CEmptyPointerClass* GetThisLuaAddress() const;

	void AddControlPointSize(Vect2f Size, Vect2f Time);
	void AddControlPointColor(CColor Color1, CColor Color2, Vect2f Time);
	void RemoveControlPointSize(int Index);
	void RemoveControlPointColor(int Index);

	CEmptyPointerClass* GetControlPointColorTimeLuaAddress(unsigned int index) const;
	CEmptyPointerClass* GetControlPointColorColor1LuaAddress(unsigned int index) const;
	CEmptyPointerClass* GetControlPointColorColor2LuaAddress(unsigned int index) const;

	CEmptyPointerClass* GetControlPointSizeTimeLuaAddress(unsigned int index) const;
	CEmptyPointerClass* GetControlPointSizeSizeLuaAddress(unsigned int index) const;

	std::vector<CParticleSystemType::ControlPointSize>&  GetLUAControlPointsSize();
	std::vector<CParticleSystemType::ControlPointColor>&  GetLUAControlPointsColor();
	int  CParticleSystemType::GetLUAControlPointsSizeSize() const;
	int  CParticleSystemType::GetLUAControlPointsColorSize() const;
	
};

#endif