#include "Camera\CameraController.h"

#include <cmath>
#include "Utils\Utils.h"
#include "XML\XMLTreeNode.h"

CCameraController::CCameraController()
:m_Yaw(0.0f)
,m_Pitch(0.0f)
,m_Position(0, 0, 0)
{
}

CCameraController::CCameraController(CXMLTreeNode &TreeNode)
:m_Position(TreeNode.GetVect3fProperty("position", v3fZERO))
,m_Yaw(TreeNode.GetFloatProperty("yaw", 0.0f))
,m_Pitch(TreeNode.GetFloatProperty("pitch", 0.0f))
{

}

CCameraController::~CCameraController()
{

}

Vect3f CCameraController::GetRight() const
{
	//return Vect3f(cos(m_Yaw-(FLOAT_PI_VALUE * 0.5f)), 0.0f, sin(m_Yaw-(FLOAT_PI_VALUE * 0.5f)));
	return Vect3f(cos(m_Pitch) * cos(m_Yaw), sin(m_Pitch) * cos(m_Yaw), -sin(m_Yaw));
}

Vect3f CCameraController::GetUp() const
{
	return Vect3f(-cos(m_Yaw)*sin(m_Pitch), cos(m_Pitch), -sin(m_Yaw)*sin(m_Pitch));
}

Vect3f CCameraController::GetForward() const
{
	return Vect3f(sin(m_Pitch) * sin(0.0f) + cos(m_Pitch) * sin(m_Yaw) * cos(0.0f), -cos(m_Pitch) * sin(0.0f) + sin(m_Pitch) * sin(m_Yaw) * cos(0.0f), cos(m_Yaw) * cos(0.0f));
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

void CCameraController::Update(float ElapsedTime)
{

}
