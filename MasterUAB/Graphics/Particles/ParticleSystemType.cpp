#include "Particles\ParticleSystemType.h"
#include "XML\XMLTreeNode.h"
#include "Materials\MaterialManager.h"
#include "Engine.h"

CParticleSystemType::CParticleSystemType(CXMLTreeNode &TreeNode) : CNamed(TreeNode)
{
	std::string l_MaterialName = TreeNode.GetPszProperty("material");
	m_Material = CEngine::GetSingleton().GetMaterialManager()->GetResource(l_MaterialName);

	m_NumFrames = TreeNode.GetIntProperty("num_frames");
	m_TimerPerFrame = TreeNode.GetFloatProperty("time_per_frame");
	m_LoopFrames = TreeNode.GetBoolProperty("loop_frames", true);
	
	m_StartingDirectionAngle = TreeNode.GetFloatProperty("starting_dir_angle");
	m_StartingAccelerationAngle = TreeNode.GetFloatProperty("starting_acc_angle");
	m_Size = TreeNode.GetVect2fProperty("size", v2fZERO);
	m_EmitRate = TreeNode.GetVect2fProperty("emit_rate", v2fZERO);
	m_AwakeTime = TreeNode.GetVect2fProperty("awake_time", v2fZERO);
	m_SleepTime = TreeNode.GetVect2fProperty("sleep_time", v2fZERO);
	m_Life = TreeNode.GetVect2fProperty("life", v2fZERO);
	m_StartingAngle = TreeNode.GetVect2fProperty("starting_angle", v2fZERO);
	m_StartingAngularSpeed = TreeNode.GetVect2fProperty("starting_ang_speed", v2fZERO);
	m_AngularAcceleration = TreeNode.GetVect2fProperty("angular_acc", v2fZERO);
	m_StartingSpeed1 = TreeNode.GetVect3fProperty("starting_speed_1", v3fZERO);
	m_StartingSpeed2 = TreeNode.GetVect3fProperty("starting_speed_2", v3fZERO);
	m_StartingAcceleration1 = TreeNode.GetVect3fProperty("starting_acc_1", v3fZERO);
	m_StartingAcceleration2 = TreeNode.GetVect3fProperty("starting_acc_2", v3fZERO);
	m_Color1 = CColor(TreeNode.GetVect4fProperty("color1", v4fZERO));
	m_Color2 = CColor(TreeNode.GetVect4fProperty("color2", v4fZERO));
	
	for (int i = 0; i < TreeNode.GetNumChildren(); ++i)
	{
		CXMLTreeNode l_Element = TreeNode(i);

		if (l_Element.GetName() == std::string("control_point_color"))
		{
			ControlPointColor l_Point;
			l_Point.m_Color1 = CColor(l_Element.GetVect4fProperty("color1",v4fZERO));
			l_Point.m_Color2 = CColor(l_Element.GetVect4fProperty("color2", v4fZERO));
			l_Point.m_Time = l_Element.GetFloatProperty("time", 0.0f);
			m_ControlPointColor.push_back(l_Point);
		}
		else if (l_Element.GetName() == std::string("control_point_size"))
		{
			ControlPointSize l_Point;
			l_Point.m_Size = l_Element.GetVect2fProperty("size", v2fZERO);
			l_Point.m_Time = l_Element.GetFloatProperty("time", 0.0f);
			m_ControlPointSizes.push_back(l_Point);
		}
	}
}

CParticleSystemType::~CParticleSystemType()
{
	m_ControlPointColor.clear();
	m_ControlPointSizes.clear();
}
