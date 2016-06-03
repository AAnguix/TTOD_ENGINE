#include "Camera\ThirdPersonCameraController.h"
#include "Camera\Camera.h"
#include "XML\XMLTreeNode.h"
#include "Engine.h"
#include "RenderableObjects\LayerManager.h"
#include "Animation\AnimatedInstanceModel.h"

const float YAW_SPEED = 1.0f;
const float PITCH_SPEED = 60.0f;
const float ROLL_SPEED = 60.0f;
const float SPEED = 5.0f;
const float FAST_SPEED = 10.0f;

const float ZOOM = 2.0f;
const float ZOOM_SPEED = 2.0f;
const float MAX_ZOOM = 5.0f;
const float MIN_ZOOM = 1.0f;

const float LOOK_AT_PITCH = 1.6f;
const float MAX_PITCH = 2.5f;
const float MIN_PITCH = -0.1f;

const float PI = 3.14159f;
const float Y_STRAFE = 1.6f;


CThirdPersonCameraController::CThirdPersonCameraController()
:CCameraController()
,m_YawSpeed(YAW_SPEED)
,m_PitchSpeed(PITCH_SPEED)
,m_RollSpeed(ROLL_SPEED)
,m_Speed(SPEED)
,m_FastSpeed(FAST_SPEED)
,m_Zoom(ZOOM)
,m_ZoomSpeed(ZOOM_SPEED)
,m_LookAtPitch(LOOK_AT_PITCH)
{
}

CThirdPersonCameraController::CThirdPersonCameraController(CXMLTreeNode &TreeNode)
:CCameraController(TreeNode)
,m_YawSpeed(TreeNode.GetFloatProperty("yaw_speed", YAW_SPEED))
,m_PitchSpeed(TreeNode.GetFloatProperty("pitch_speed", PITCH_SPEED))
,m_RollSpeed(TreeNode.GetFloatProperty("roll_speed", ROLL_SPEED))
,m_Speed(TreeNode.GetFloatProperty("speed", SPEED))
,m_FastSpeed(TreeNode.GetFloatProperty("fast_speed", FAST_SPEED))
,m_Zoom(TreeNode.GetFloatProperty("zoom", ZOOM))
,m_MaxZoom(TreeNode.GetFloatProperty("max_zoom", MAX_ZOOM))
,m_MinZoom(TreeNode.GetFloatProperty("min_zoom", MIN_ZOOM))
,m_ZoomSpeed(TreeNode.GetFloatProperty("zoom_speed", ZOOM_SPEED))
,m_LookAtPitch(TreeNode.GetFloatProperty("look_at_pitch", LOOK_AT_PITCH))
,m_MaxLookAtPitch(TreeNode.GetFloatProperty("max_look_at_pitch", MAX_PITCH))
,m_MinLookAtPitch(TreeNode.GetFloatProperty("min_look_at_pitch", MIN_PITCH))
{
	
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
	l_AddPos.y = 0.0f;
	l_AddPos.x = Forward*(cos(m_Yaw)) + Strafe*(cos(m_Yaw + PI*0.5f));
	l_AddPos.z = Forward*(sin(m_Yaw)) + Strafe*(sin(m_Yaw + PI*0.5f));

	float l_ConstantSpeed = ElapsedTime*m_Speed;
	if (Speed)
		l_ConstantSpeed *= m_FastSpeed;

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
	l_AddPos.x = 0.0f;
	l_AddPos.y = Movement;
	l_AddPos.z = 0.0f;

	float l_ConstantSpeed = ElapsedTime*m_Speed;
	if (Speed)
		l_ConstantSpeed *= m_FastSpeed;

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
	CCameraController::AddYaw(Radians * m_YawSpeed);
}

void CThirdPersonCameraController::AddPitch(float Radians)
{
	CCameraController::AddPitch(Radians*m_PitchSpeed);
}

void CThirdPersonCameraController::SetCamera(CCamera *Camera) const
{
	Vect3f l_PlayerPos = m_Position;

	CRenderableObject* l_Player = CEngine::GetSingleton().GetLayerManager()->GetPlayer();
	assert(l_Player != nullptr);

	if (l_Player != nullptr)
	{
		l_PlayerPos = l_Player->GetPosition();
		l_PlayerPos.y = l_PlayerPos.y + Y_STRAFE;
	}
	Vect3f l_Direction;

	l_Direction = GetDirection();
	Vect3f l_Position = l_PlayerPos - l_Direction;
	l_Position.y = l_Position.y + m_LookAtPitch;

	Camera->SetFOV(FOV);
	Camera->SetAspectRatio(ASPECT_RATIO);
	Camera->SetPosition(l_Position);
	Camera->SetLookAt(l_PlayerPos);
	Camera->SetUp(GetUp());
	Camera->SetZNear(ZNEAR);
	Camera->SetZFar(ZFAR);
	Camera->SetMatrixs();

}

Vect3f CThirdPersonCameraController::GetDirection() const
{
	//return Vect3f(cos(m_Yaw)*cos(m_Pitch), sin(m_Pitch), sin(m_Yaw)*cos(m_Pitch));
	//return Vect3f(m_Zoom*cos(m_Yaw)*cos(m_Pitch), m_Zoom*sin(m_Pitch), m_Zoom*sin(m_Yaw)*cos(m_Pitch));
	Vect3f l_Direction = CCameraController::GetForward();
	l_Direction.x = m_Zoom*l_Direction.x;
	l_Direction.y = m_Zoom*l_Direction.y;
	l_Direction.z = m_Zoom*l_Direction.z;
	return l_Direction;
	//return Vect3f(m_Zoom*-cos(m_Pitch)*sin(m_Yaw), m_Zoom*sin(m_Pitch), m_Zoom*cos(m_Pitch)*cos(m_Yaw));

	//return Vect3f(m_Zoom*-cos(m_Pitch)*sin(m_Yaw), m_Zoom*sin(m_Pitch), m_Zoom*cos(m_Pitch)*cos(m_Yaw));

	//return Vect3f(m_Zoom*-cos(m_Pitch)*sin(0.0), m_Zoom*sin(0.0), m_Zoom*cos(0.0)*cos(m_Yaw));
	//VECTOR3D(-cos(m_pitch * M_PI / 180) * sin(m_yaw * M_PI / 180), sin(m_pitch * M_PI / 180), cos(m_pitch * M_PI / 180) * cos(m_yaw * M_PI / 180));


}



//void CThirdPersonCameraController::Update(Vect3f Rotation)
//{
//	AddYaw(Rotation.x);
//	//AddPitch(Rotation.y*1.0f);
//	//AddZoom(-Rotation.z*2.0f);
//}

void CThirdPersonCameraController::AddZoom(float Zoom)
{
	m_Zoom += Zoom;
	if (m_Zoom > m_MaxZoom)
	{
		m_Zoom = m_MaxZoom;
	}
	else if (m_Zoom < m_MinZoom)
	{
		m_Zoom = m_MinZoom;
	}
}

void CThirdPersonCameraController::AddLookAtPitch(float Pitch)
{
	m_LookAtPitch += Pitch;
	if (m_LookAtPitch > m_MaxLookAtPitch)
	{
		m_LookAtPitch = m_MaxLookAtPitch;
	}
	else if (m_LookAtPitch < m_MinLookAtPitch)
	{
		m_LookAtPitch = m_MinLookAtPitch;
	}
}

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
	if (dot>1.0)dot = 1.0;
	else if (dot<-1.0)dot = -1.0;

	float angle = acos(dot);

	Vect3f cross = PlayerForward^l_CameraForward;
	if (cross.y<0.0f)
		angle = angle*-1;

	return angle;

}