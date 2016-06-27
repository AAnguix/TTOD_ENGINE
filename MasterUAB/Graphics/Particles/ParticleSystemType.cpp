#include "Particles\ParticleSystemType.h"
#include "XML\XMLTreeNode.h"
#include "Materials\MaterialManager.h"
#include "Engine\Engine.h"
#include "Utils\EmptyPointerClass.h"
#include "Materials\Material.h"
#include "Log\Log.h"

CParticleSystemType::CParticleSystemType(CXMLTreeNode &TreeNode) : CNamed(TreeNode)
,m_NumFrames(TreeNode.GetIntProperty("num_frames"))
,m_TimerPerFrame(TreeNode.GetFloatProperty("time_per_frame"))
,m_LoopFrames(TreeNode.GetBoolProperty("loop_frames", false))
,m_EmitAbsolute(TreeNode.GetBoolProperty("emit_absolute",false))
,m_StartingDirectionAngle(TreeNode.GetFloatProperty("starting_dir_angle"))
,m_StartingAccelerationAngle(TreeNode.GetFloatProperty("starting_acc_angle"))
,m_Size(TreeNode.GetVect2fProperty("size", v2fZERO))
,m_EmitRate(TreeNode.GetVect2fProperty("emit_rate", v2fZERO))
,m_AwakeTime(TreeNode.GetVect2fProperty("awake_time", v2fZERO))
,m_SleepTime(TreeNode.GetVect2fProperty("sleep_time", v2fZERO))
,m_Life(TreeNode.GetVect2fProperty("life", v2fZERO))
,m_StartingAngle(TreeNode.GetVect2fProperty("starting_angle", v2fZERO))
,m_StartingAngularSpeed(TreeNode.GetVect2fProperty("starting_ang_speed", v2fZERO))
,m_AngularAcceleration(TreeNode.GetVect2fProperty("angular_acc", v2fZERO))
,m_StartingSpeed1(TreeNode.GetVect3fProperty("starting_speed_1", v3fZERO))
,m_StartingSpeed2(TreeNode.GetVect3fProperty("starting_speed_2", v3fZERO))
,m_StartingAcceleration1(TreeNode.GetVect3fProperty("starting_acc_1", v3fZERO))
,m_StartingAcceleration2(TreeNode.GetVect3fProperty("starting_acc_2", v3fZERO))
,m_Color1(CColor(TreeNode.GetVect4fProperty("color1", v4fZERO)))
,m_Color2(CColor(TreeNode.GetVect4fProperty("color2", v4fZERO)))
,m_Material(CEngine::GetSingleton().GetMaterialManager()->GetResource(TreeNode.GetPszProperty("material")))
{
	assert(m_Material != nullptr);

	if (m_NumFrames < 1) m_NumFrames = 1;

	for (int i = 0; i < TreeNode.GetNumChildren(); ++i)
	{
		CXMLTreeNode l_Element = TreeNode(i);

		if (l_Element.GetName() == std::string("control_point_color"))
		{
			ControlPointColor l_Point(
			CColor(l_Element.GetVect4fProperty("color1", v4fZERO)), CColor(l_Element.GetVect4fProperty("color2", v4fZERO)), l_Element.GetVect2fProperty("time", v2fZERO));
			m_ControlPointColor.push_back(l_Point);
		}
		else if (l_Element.GetName() == std::string("control_point_size"))
		{
			ControlPointSize l_Point(l_Element.GetVect2fProperty("size", v2fZERO), l_Element.GetVect2fProperty("time", v2fZERO));
			m_ControlPointSizes.push_back(l_Point);
		}
	}

	#ifdef _DEBUG
	if (m_ControlPointSizes.size() < 1 || m_ControlPointColor.size() < 1)
		CEngine::GetSingleton().GetLogManager()->Log("Particle systems should have at least one control point (size and color) "+m_Name);
	#endif

	assert(m_ControlPointSizes.size() > 0);
	assert(m_ControlPointColor.size() > 0);
}

CParticleSystemType::~CParticleSystemType()
{
	m_ControlPointColor.clear();
	m_ControlPointSizes.clear();
}

CEmptyPointerClass* CParticleSystemType::GetNumFramesLuaAddress() const { return (CEmptyPointerClass *)((void*)&m_NumFrames); }
CEmptyPointerClass* CParticleSystemType::GetTimePerFrameLuaAddress() const { return (CEmptyPointerClass *)((void*)&m_TimerPerFrame); }
CEmptyPointerClass* CParticleSystemType::GetLoopLuaAddress() const { return (CEmptyPointerClass *)((void*)&m_LoopFrames); }
CEmptyPointerClass* CParticleSystemType::GetEmitAbsoluteLuaAddress() const { return (CEmptyPointerClass *)((void*)&m_EmitAbsolute); }

CEmptyPointerClass* CParticleSystemType::GetStartingDirectionAngleLuaAddress() const { return (CEmptyPointerClass *)((void*)&m_StartingDirectionAngle); }
CEmptyPointerClass* CParticleSystemType::GetStartingAccelerationAngleLuaAddress() const { return (CEmptyPointerClass *)((void*)&m_StartingAccelerationAngle); }
CEmptyPointerClass* CParticleSystemType::GetSizeLuaAddress(int Index) { return (CEmptyPointerClass *)&m_Size[Index]; }

CEmptyPointerClass* CParticleSystemType::GetEmitRateLuaAddress(int Index) { return (CEmptyPointerClass *)&m_EmitRate[Index]; }
CEmptyPointerClass* CParticleSystemType::GetAwakeTimeLuaAddress(int Index) { return (CEmptyPointerClass *)&m_AwakeTime[Index]; }
CEmptyPointerClass* CParticleSystemType::GetSleepTimeLuaAddress(int Index) { return (CEmptyPointerClass *)&m_SleepTime[Index]; }
CEmptyPointerClass* CParticleSystemType::GetLifeLuaAddress(int Index) { return (CEmptyPointerClass *)&m_Life[Index]; }

CEmptyPointerClass* CParticleSystemType::GetStartingAngleLuaAddress(int Index) { return (CEmptyPointerClass *)&m_StartingAngle[Index]; }
CEmptyPointerClass* CParticleSystemType::GetStartingAngularSpeedLuaAddress(int Index) { return (CEmptyPointerClass *)&m_StartingAngularSpeed[Index]; }
CEmptyPointerClass* CParticleSystemType::GetAngularAccelerationLuaAddress(int Index) { return (CEmptyPointerClass *)&m_AngularAcceleration[Index]; }

CEmptyPointerClass* CParticleSystemType::GetStartingSpeed1LuaAddress(int Index) { return (CEmptyPointerClass *)&m_StartingSpeed1[Index]; }
CEmptyPointerClass* CParticleSystemType::GetStartingSpeed2LuaAddress(int Index) { return (CEmptyPointerClass *)&m_StartingSpeed2[Index]; }
CEmptyPointerClass* CParticleSystemType::GetStartingAcceleration1LuaAddress(int Index) { return (CEmptyPointerClass *)&m_StartingAcceleration1[Index]; }
CEmptyPointerClass* CParticleSystemType::GetStartingAcceleration2LuaAddress(int Index) { return (CEmptyPointerClass *)&m_StartingAcceleration2[Index]; }
CEmptyPointerClass* CParticleSystemType::GetColor1LuaAddress() const { return (CEmptyPointerClass *)((void*)&m_Color1); }
CEmptyPointerClass* CParticleSystemType::GetColor2LuaAddress() const { return (CEmptyPointerClass *)((void*)&m_Color2); }
CEmptyPointerClass* CParticleSystemType::GetThisLuaAddress() const { return (CEmptyPointerClass *)((void*)this); }

//CEmptyPointerClass* CParticleSystemType::ControlPointSize::GetSizeLuaAddress() const { return (CEmptyPointerClass *)((void*)&m_Size); }
//CEmptyPointerClass* CParticleSystemType::ControlPointSize::GetTimeLuaAddress() const { return (CEmptyPointerClass *)((void*)&m_Time); }
//CEmptyPointerClass* CParticleSystemType::ControlPointSize::GetThisLuaAddress() const { return (CEmptyPointerClass *)((void*)this); }
//
//CEmptyPointerClass* CParticleSystemType::ControlPointColor::GetTimeLuaAddress() const { return (CEmptyPointerClass *)((void*)&m_Time); }
//CEmptyPointerClass* CParticleSystemType::ControlPointColor::GetColor1LuaAddress() const { return (CEmptyPointerClass *)((void*)&m_Color1); }
//CEmptyPointerClass* CParticleSystemType::ControlPointColor::GetColor2LuaAddress() const { return (CEmptyPointerClass *)((void*)&m_Color2); }
//CEmptyPointerClass* CParticleSystemType::ControlPointColor::GetThisLuaAddress() const { return (CEmptyPointerClass *)((void*)this); }

CEmptyPointerClass* CParticleSystemType::GetControlPointColorTimeLuaAddress(unsigned int Index) const { return (CEmptyPointerClass *)((void*)&m_ControlPointColor[Index].m_Time); }
CEmptyPointerClass* CParticleSystemType::GetControlPointColorColor1LuaAddress(unsigned int Index) const { return (CEmptyPointerClass *)((void*)&m_ControlPointColor[Index].m_Color1); }
CEmptyPointerClass* CParticleSystemType::GetControlPointColorColor2LuaAddress(unsigned int Index) const { return (CEmptyPointerClass *)((void*)&m_ControlPointColor[Index].m_Color2); }

CEmptyPointerClass* CParticleSystemType::GetControlPointSizeTimeLuaAddress(unsigned int Index) const { return (CEmptyPointerClass *)((void*)&m_ControlPointSizes[Index].m_Time); }
CEmptyPointerClass* CParticleSystemType::GetControlPointSizeSizeLuaAddress(unsigned int Index) const { return (CEmptyPointerClass *)((void*)&m_ControlPointSizes[Index].m_Size); }


void CParticleSystemType::AddControlPointSize(Vect2f Size, Vect2f Time)
{
	m_ControlPointSizes.push_back(ControlPointSize(Size, Time));
}

void CParticleSystemType::AddControlPointColor(CColor Color1, CColor Color2, Vect2f Time)
{
	m_ControlPointColor.push_back(ControlPointColor(Color1, Color2, Time));
}

void CParticleSystemType::RemoveControlPointSize(int Index)
{
	assert(Index > -1);
	m_ControlPointSizes.erase(m_ControlPointSizes.begin() + Index);
}
void CParticleSystemType::RemoveControlPointColor(int Index)
{
	assert(Index > -1);
	m_ControlPointColor.erase(m_ControlPointColor.begin() + Index);
}

//
std::vector<CParticleSystemType::ControlPointSize>&  CParticleSystemType::GetLUAControlPointsSize()
{
	return m_ControlPointSizes;
}

std::vector<CParticleSystemType::ControlPointColor>&  CParticleSystemType::GetLUAControlPointsColor()
{
	return m_ControlPointColor;
}

int  CParticleSystemType::GetLUAControlPointsSizeSize() const
{
	return m_ControlPointSizes.size();
}

int  CParticleSystemType::GetLUAControlPointsColorSize() const
{
	return m_ControlPointColor.size();
}