#include "Camera\ThirdPersonCameraController.h"
#include "Camera\Camera.h"
#include "XML\XMLTreeNode.h"
#include "Engine.h"
#include "RenderableObjects\LayerManager.h"

CThirdPersonCameraController::CThirdPersonCameraController()
: m_YawSpeed(50.f)
, m_PitchSpeed(60.f)
, m_RollSpeed(60.f)
, m_Speed(5.0f)
, m_FastSpeed(10.0f)
, m_Zoom(3.0f)
, m_ZoomSpeed(2.f)
{
	m_Position=v3fZERO;
	m_Position=Vect3f(0.0f,1.0f,0.0f);
	
}
CThirdPersonCameraController::CThirdPersonCameraController(CXMLTreeNode &TreeNode)
: m_YawSpeed(50.f)
, m_PitchSpeed(60.f)
, m_RollSpeed(60.f)
, m_Speed(5.0f)
, m_FastSpeed(10.0f)
, m_Zoom(3.0f)
, m_ZoomSpeed(2.f)
{
	m_Position=TreeNode.GetVect3fProperty("position",v3fZERO);
	
}


CThirdPersonCameraController::~CThirdPersonCameraController()
{	
}


void CThirdPersonCameraController::Move(float Strafe, float Forward, bool Speed, float ElapsedTime)
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

void CThirdPersonCameraController::MoveUpDown(float Movement, bool Speed, float ElapsedTime)
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

void CThirdPersonCameraController::AddYaw(float Radians)
{
	//CCameraController::AddYaw(-Radians*m_YawSpeed);
	CCameraController::AddYaw(Radians);
}

void CThirdPersonCameraController::AddPitch(float Radians)
{
	CCameraController::AddPitch(Radians*m_PitchSpeed);
}

void CThirdPersonCameraController::SetCamera(CCamera *Camera) const
{
	/*
	Vect3f l_Direction = GetDirection();
	Camera->SetFOV(1.047f);
	Camera->SetAspectRatio(16.0f/9.0f);
	Camera->SetPosition(m_Position);
	Camera->SetLookAt(m_Position+l_Direction);
	Camera->SetUp(GetUp());
	Camera->SetZNear(0.1f);
	Camera->SetZFar(100.f);
	Camera->SetMatrixs();
	*/

	Vect3f l_PlayerPos=m_Position;
	CRenderableObject* l_Player = CEngine::GetSingleton().GetLayerManager()->GetResource("solid")->GetResource("bruja");
	
	Vect3f T_CameraForward = CCameraController::GetForward();
	Vect3f T_PlayerForward = l_Player->GetForward();
	Vect3f T_PlayerTranslation = l_Player->GetTranslation();
	
	if(l_Player != NULL)
	{
		l_PlayerPos = l_Player->GetPosition();
		l_PlayerPos.y = l_PlayerPos.y + 1.0f;
	}
	Vect3f l_Direction;
	
	l_Direction = GetDirection();
	Vect3f l_Position = l_PlayerPos-l_Direction;
	
	Camera->SetFOV(1.047f);
	Camera->SetAspectRatio(16.0f/9.0f);
	Camera->SetPosition(l_Position);
	Camera->SetLookAt(l_PlayerPos);
	Camera->SetUp(GetUp());
	Camera->SetZNear(0.1f);
	Camera->SetZFar(100.f);
	Camera->SetMatrixs();
	
}

Vect3f CThirdPersonCameraController::GetDirection() const
{
	//return Vect3f(cos(m_Yaw)*cos(m_Pitch), sin(m_Pitch), sin(m_Yaw)*cos(m_Pitch));
	//return Vect3f(m_Zoom*cos(m_Yaw)*cos(m_Pitch), m_Zoom*sin(m_Pitch), m_Zoom*sin(m_Yaw)*cos(m_Pitch));
	Vect3f l_Direction= CCameraController::GetForward();
	l_Direction.x = m_Zoom*l_Direction.x;
	l_Direction.y = m_Zoom*l_Direction.y;
	l_Direction.z = m_Zoom*l_Direction.z;
	return l_Direction;
	//return Vect3f(m_Zoom*-cos(m_Pitch)*sin(m_Yaw), m_Zoom*sin(m_Pitch), m_Zoom*cos(m_Pitch)*cos(m_Yaw));
	
	//return Vect3f(m_Zoom*-cos(m_Pitch)*sin(m_Yaw), m_Zoom*sin(m_Pitch), m_Zoom*cos(m_Pitch)*cos(m_Yaw));
	
	//return Vect3f(m_Zoom*-cos(m_Pitch)*sin(0.0), m_Zoom*sin(0.0), m_Zoom*cos(0.0)*cos(m_Yaw));
	//VECTOR3D(-cos(m_pitch * M_PI / 180) * sin(m_yaw * M_PI / 180), sin(m_pitch * M_PI / 180), cos(m_pitch * M_PI / 180) * cos(m_yaw * M_PI / 180));

	
}

CThirdPersonCameraController::EType CThirdPersonCameraController::GetType() const
{
	return FIXED;
}


void CThirdPersonCameraController::Update(Vect3f Rotation)
	{
		AddYaw(Rotation.x*1.0f);
		//AddPitch(Rotation.y*1.0f);
		//AddZoom(-Rotation.z*2.0f);
	}

void CThirdPersonCameraController::AddZoom(float Zoom) 
	{m_Zoom+=Zoom;}

float CThirdPersonCameraController::GetPlayerCameraAngleDif(Vect3f PlayerForward) 
{
	Vect3f l_CameraForward = CCameraController::GetForward();
	
	/*
	Vect3f l_CameraPosition = CCameraController::GetPosition();
	
	Vect3f distance = PlayerPosition - l_CameraPosition;
	distance.Normalize();
	*/

	l_CameraForward.Normalize();
	PlayerForward.Normalize();

	float dot = l_CameraForward*PlayerForward;
	if(dot>1.0)dot=1.0;
	else if(dot<-1.0)dot=-1.0;

	float angle = acos(dot);

	Vect3f cross = PlayerForward^l_CameraForward;
	if(cross.y<0.0f)
		angle=angle*-1;

	return angle;

}