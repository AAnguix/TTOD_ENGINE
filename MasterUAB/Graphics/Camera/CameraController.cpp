#include "Camera\CameraController.h"

#include <cmath>
#include "Utils\Utils.h"
#include "XML\XMLTreeNode.h"

CCameraController::CCameraController()
: m_Yaw(0.0f)
, m_Pitch(0.0f)
, m_Position(0, 0, 0)
{
}

CCameraController::CCameraController(CXMLTreeNode &TreeNode)
{
	m_Position = TreeNode.GetVect3fProperty("position", v3fZERO);
	m_FOV = TreeNode.GetFloatProperty("fov", 1.0f);
	m_Aspect = TreeNode.GetFloatProperty("aspect", 1.7778f);
	m_ZFar = TreeNode.GetFloatProperty("far", 100.0f);
	m_ZNear = TreeNode.GetFloatProperty("near", 0.1f);
}

CCameraController::~CCameraController()
{

}

Vect3f CCameraController::GetRight() const
{
	//return Vect3f(cos(m_Yaw-(FLOAT_PI_VALUE * 0.5f)), 0.0f, sin(m_Yaw-(FLOAT_PI_VALUE * 0.5f)));
	return Vect3f(cos(m_Pitch) * cos(m_Yaw),
		sin(m_Pitch) * cos(m_Yaw),
		-sin(m_Yaw)
		);
}

Vect3f CCameraController::GetUp() const
{
	Vect3f l_Up(-cos(m_Yaw)*sin(m_Pitch), cos(m_Pitch), -sin(m_Yaw)*sin(m_Pitch));
	return l_Up;
}

Vect3f CCameraController::GetForward() const
{
	return Vect3f(sin(m_Pitch) * sin(0.0) + cos(m_Pitch) * sin(m_Yaw) * cos(0.0),
		-cos(m_Pitch) * sin(0.0) + sin(m_Pitch) * sin(m_Yaw) * cos(0.0),
		cos(m_Yaw) * cos(0.0));
}

void CCameraController::AddYaw(float Radians)
{
	m_Yaw += Radians;
	if (m_Yaw>2 * DEG2RAD(180.0f))
		m_Yaw -= 2 * DEG2RAD(180.0f);
	else if (m_Yaw<-2 * DEG2RAD(180.0f))
		m_Yaw += 2 * DEG2RAD(180.0f);
}
void CCameraController::AddPitch(float Radians)
{
	if (((m_Pitch - Radians)<(DEG2RAD(180.0f)*0.5) && (m_Pitch - Radians)>-(DEG2RAD(180.0f)*0.5)))
	{
		m_Pitch -= Radians;
	}
}

CCameraController::EType CCameraController::GetType() const
{
	return CCameraController::FIXED;
}
