#include <math.h>
#include <stdio.h>
#include "Camera\SphericalCameraController.h"
#include "Camera\Camera.h"
#include "Utils\Utils.h"

CSphericalCameraController::CSphericalCameraController() 
: m_Zoom(75.0f)
, m_ZoomSpeed(2.f)
{

}

CSphericalCameraController::~CSphericalCameraController()
{	
}

Vect3f CSphericalCameraController::GetDirection() const
{
	return Vect3f(m_Zoom*cos(m_Yaw)*cos(m_Pitch), m_Zoom*sin(m_Pitch), m_Zoom*sin(m_Yaw)*cos(m_Pitch));
}

void CSphericalCameraController::SetCamera(CCamera *Camera) const
{
	Vect3f l_Direction = GetDirection();

	Camera->SetLookAt(m_Position);

	Camera->SetPosition(m_Position-l_Direction);

	Camera->SetUp(GetUp());
	Camera->SetMatrixs();
}
