#include "Camera\FixedCameraController.h"
#include "Camera\Camera.h"
#include "XML\XMLTreeNode.h"

CFixedCameraController::CFixedCameraController()
{
	m_Position=v3fZERO;
	m_LookAt=v3fZERO;
	m_FOV=0.0f;
	m_Near=0.0f;
	m_Far=0.0f;
}

CFixedCameraController::CFixedCameraController(CXMLTreeNode &TreeNode)
{
	m_Position=TreeNode.GetVect3fProperty("position",v3fZERO);
	m_LookAt=TreeNode.GetVect3fProperty("look_at",v3fZERO);
	m_FOV=TreeNode.GetFloatProperty("fov",0.0f);
	m_Near=TreeNode.GetFloatProperty("near",0.0f);
	m_Far=TreeNode.GetFloatProperty("far",0.0f);
}

CFixedCameraController::~CFixedCameraController()
{	
}

void CFixedCameraController::SetCamera(CCamera *Camera) const
{
	Vect3f l_Direction = GetDirection();
	Camera->SetFOV(m_FOV);
	Camera->SetPosition(m_Position);
	Camera->SetLookAt(m_LookAt);
	Camera->SetZNear(m_Near);
	Camera->SetZFar(m_Far);
	Camera->SetUp(GetUp());
	Camera->SetMatrixs();
}

Vect3f CFixedCameraController::GetDirection() const
{
	Vect3f l_Direction(cos(m_Yaw)*cos(m_Pitch), sin(m_Pitch), sin(m_Yaw)*cos(m_Pitch));
	return l_Direction;
}