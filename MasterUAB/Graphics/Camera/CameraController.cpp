#include "Camera\CameraController.h"

#include <cmath>
#include "Utils\Utils.h"
#include "XML\XMLTreeNode.h"
#include "Camera\CameraConstants.h"

CCameraController::CCameraController()
:CNamed("")
,m_Yaw(0.0f)
,m_Pitch(0.0f)
,m_Position(0, 0, 0)
,m_Fov(FOV)
,m_AspectRatio(ASPECT_RATIO)
,m_ZNear(ZNEAR)
,m_ZFar(ZFAR)
{
}

CCameraController::CCameraController(CXMLTreeNode &TreeNode)
:CNamed(TreeNode)
,m_Position(TreeNode.GetVect3fProperty("position", v3fZERO))
,m_Yaw(TreeNode.GetFloatProperty("yaw", 0.0f))
,m_Pitch(TreeNode.GetFloatProperty("pitch", 0.0f))
,m_Fov(TreeNode.GetFloatProperty("fov", FOV))
,m_AspectRatio(TreeNode.GetFloatProperty("aspect_ratio", ASPECT_RATIO))
,m_ZNear(TreeNode.GetFloatProperty("z_near", ZNEAR))
,m_ZFar(TreeNode.GetFloatProperty("z_far", ZFAR))
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
	//return Vect3f(sin(m_Pitch) * sin(0.0f) + cos(m_Pitch) * sin(m_Yaw) * cos(0.0f), -cos(m_Pitch) * sin(0.0f) + sin(m_Pitch) * sin(m_Yaw) * cos(0.0f), cos(m_Yaw) * cos(0.0f));
	return Vect3f(cos(m_Pitch) * sin(m_Yaw),sin(m_Pitch) * sin(m_Yaw), cos(m_Yaw));
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

void CCameraController::Update(float ElapsedTime){}

CEmptyPointerClass* CCameraController::GetThisLuaAddress() const { return (CEmptyPointerClass *)this; }