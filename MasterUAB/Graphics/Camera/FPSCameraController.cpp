#include "Camera\FPSCameraController.h"
#include "Camera\Camera.h"
#include "XML\XMLTreeNode.h"

CFPSCameraController::CFPSCameraController()
:CCameraController()
,m_YawSpeed(1.f)
,m_PitchSpeed(1.0f)
,m_RollSpeed(1.0f)
,m_Speed(5.0f)
,m_FastSpeed(10.0f)
{
	m_Position=v3fZERO;
	m_Position=Vect3f(0.0f,15.0f,0.0f);
}
CFPSCameraController::CFPSCameraController(CXMLTreeNode &TreeNode)
:CCameraController(TreeNode)
,m_YawSpeed(TreeNode.GetFloatProperty("yaw_speed", 1.0f))
,m_PitchSpeed(TreeNode.GetFloatProperty("pitch_speed", 1.0f))
,m_RollSpeed(1.0f)
,m_Speed(TreeNode.GetFloatProperty("speed", 1.0f))
,m_FastSpeed(TreeNode.GetFloatProperty("fast_speed", 1.0f))
{
}

CFPSCameraController::~CFPSCameraController()
{	
}

/*
void CFPSCameraController::Move(float Strafe, float Forward, bool Speed, float ElapsedTime)
{
	//Forward=2.0f;
	//Strafe=3.0f;
	//Speed=true;
	Vect3f l_AddPos;
	l_AddPos.y=0.0f;
	l_AddPos.x=Forward*(cos(m_Yaw))+Strafe*(cos(m_Yaw+3.14159f*0.5f));
	l_AddPos.z=Forward*(sin(m_Yaw))+Strafe*(sin(m_Yaw+3.14159f*0.5f));
	
	float l_ConstantSpeed=ElapsedTime*m_Speed;
	if(Speed)
		l_ConstantSpeed*=m_FastSpeed;

	if (l_AddPos.SquaredLength() > 0)
	{
		l_AddPos.Normalize();
		l_AddPos *= l_ConstantSpeed;
		m_Position += l_AddPos;
	}
}*/

void CFPSCameraController::Move(float Strafe, float Forward, bool Speed, float ElapsedTime)
{
	//Forward=2.0f;
	//Strafe=3.0f;
	//Speed=true;
	Vect3f l_AddPos;
	l_AddPos.y=0.0f;
	l_AddPos.x=Forward*(cos(m_Yaw))+Strafe*(cos(m_Yaw+3.14159f*0.5f));
	l_AddPos.z=Forward*(sin(m_Yaw))+Strafe*(sin(m_Yaw+3.14159f*0.5f));
	
	float l_ConstantSpeed=ElapsedTime*m_Speed;
	if(Speed)
		l_ConstantSpeed*=m_FastSpeed;

	if (l_AddPos.SquaredLength() > 0)
	{
		l_AddPos.Normalize();
		l_AddPos *= l_ConstantSpeed;
		m_Position += l_AddPos;
	}
}

void CFPSCameraController::MoveUpDown(float Movement, bool Speed, float ElapsedTime)
{
	Vect3f l_AddPos;
	l_AddPos.x=0.0f;
	l_AddPos.y=Movement;
	l_AddPos.z=0.0f;

	float l_ConstantSpeed=ElapsedTime*m_Speed;
	if(Speed)
		l_ConstantSpeed*=m_FastSpeed;

	if (l_AddPos.SquaredLength() > 0)
	{
		l_AddPos.Normalize();
		l_AddPos *= l_ConstantSpeed;
		m_Position += l_AddPos;
	}
}

void CFPSCameraController::AddYaw(float Radians)
{
	CCameraController::AddYaw(Radians*m_YawSpeed);
}

void CFPSCameraController::AddPitch(float Radians)
{
	CCameraController::AddPitch(Radians*m_PitchSpeed);
}

void CFPSCameraController::SetCamera(CCamera *Camera) const
{
	Vect3f l_Direction = GetDirection();
	Camera->SetFOV(1.047f);
	Camera->SetAspectRatio(16.0f/9.0f);
	Camera->SetPosition(m_Position);
	Camera->SetLookAt(m_Position+l_Direction);
	Camera->SetUp(GetUp());
	Camera->SetZNear(0.1f);
	Camera->SetZFar(100.f);
	Camera->SetMatrixs();
}

Vect3f CFPSCameraController::GetDirection() const
{
	Vect3f l_Direction(cos(m_Yaw)*cos(m_Pitch), sin(m_Pitch), sin(m_Yaw)*cos(m_Pitch));
	return l_Direction;
}
