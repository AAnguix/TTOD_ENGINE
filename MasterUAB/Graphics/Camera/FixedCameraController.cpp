#include "Camera\FixedCameraController.h"
#include "Camera\Camera.h"
#include "XML\XMLTreeNode.h"

CFixedCameraController::CFixedCameraController()
{
	m_Position=v3fZERO;
	m_LookAt=v3fZERO;
}

CFixedCameraController::CFixedCameraController(CXMLTreeNode &TreeNode) : CCameraController()
{
	m_Position=TreeNode.GetVect3fProperty("position",v3fZERO);
	m_LookAt=TreeNode.GetVect3fProperty("look_at",v3fZERO);
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
	Camera->SetZNear(m_ZNear);
	Camera->SetZFar(m_ZFar);
	Camera->SetUp(GetUp());
	Camera->SetMatrixs();
}

Vect3f CFixedCameraController::GetDirection() const
{
	Vect3f l_Direction(cos(m_Yaw)*cos(m_Pitch), sin(m_Pitch), sin(m_Yaw)*cos(m_Pitch));
	return l_Direction;
}