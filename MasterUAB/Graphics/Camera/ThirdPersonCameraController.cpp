#include "XML\XMLTreeNode.h"
#include "Camera\ThirdPersonCameraController.h"
#include "Camera\Camera.h"
#include "Camera\Shaker\CameraShaker.h"
#include "Engine\Engine.h"
#include "GameObject\GameObjectManager.h"
#include "Animation\AnimatedInstanceModel.h"
#include "PhysXManager.h"
#include "PerlinNoise.h"
#include "Log\Log.h"

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
,m_PlayerPos(Vect3f(0.0, 0.0, 0.0))
,m_NewCameraPosition(Vect3f(0.0, 0.0, 0.0))
,m_Shaking(false)
{
	m_Shaker = new CCameraShaker();
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
,m_PlayerPos(Vect3f(0.0, 0.0, 0.0))
,m_NewCameraPosition(Vect3f(0.0, 0.0, 0.0))
,m_Shaking(false)
{
	m_Shaker = new CCameraShaker();
	//m_Shaker.Initialize("./Data/Cameras/camera_shaker.txt");

	/*CEngine::GetSingleton().GetPhysXManager()->RegisterMaterial("Camera_Mat", 30.0, 40.0, 0.0);
	Quatf l_QuatCamera = Quatf(0.0, 0.0, 0.0, 1.0);
	CEngine::GetSingleton().GetPhysXManager()->CreateRigidDynamicSphere("CamPhysX", 0.1, "Camera_Mat", m_NewCameraPosition, l_QuatCamera, 0, 1.0, true);
	CEngine::GetSingleton().GetPhysXManager()->MoveKinematicActor("CamPhysX", m_NewCameraPosition);*/
}

CThirdPersonCameraController::~CThirdPersonCameraController()
{
	if (m_Shaker)
	{
		delete m_Shaker;
		m_Shaker = nullptr;
	}
}

void CThirdPersonCameraController::StartSmoothing()
{
	Vect3f l_CameraRotation = Vect3f(m_Yaw, m_Pitch, 0.0f);
	m_Smoother.Start(m_Position, l_CameraRotation, Vect3f(1.2f, 0.3f, 0.0f), Quatf(0.1f, 0.2f, 0.1f), 60.0f, 0.5f, 8);
}

void CThirdPersonCameraController::StopSmoothing()
{
	m_Smoother.Stop();
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


void CThirdPersonCameraController::StartShaking(Vect3f CameraRotation, int Duration, int YawFrequency, float MinYawRandom, float MaxYawRandom, int PitchFrequency, float MinPitchRandom, float MaxPitchRandom)
{
	if (!m_Shaking)
	{
		m_Shaker->Start(CameraRotation, Duration, YawFrequency, MinYawRandom, MaxYawRandom, PitchFrequency, MinPitchRandom, MaxPitchRandom);
		m_Shaking = true;
	}
}

void CThirdPersonCameraController::Update(float ElapsedTime)
{
	//m_Smoother.Update(ElapsedTime, this);
	if (m_Shaking)
		m_Shaker->Update(ElapsedTime, this);

	if (m_Shaking && m_Shaker->Finish())
	{
		m_Shaker->Stop(m_Yaw, m_Pitch);
		m_Shaking = false;
	}

	m_CurrentTime += ElapsedTime;
	m_PlayerPos = m_Position;

	CGameObject* l_PlayerGameObject = CEngine::GetSingleton().GetGameObjectManager()->GetPlayer();
	if (l_PlayerGameObject)
	{
		CRenderableObject* l_Player = l_PlayerGameObject->GetRenderableObject();
		assert(l_Player != nullptr);

		if (l_Player != nullptr)
		{
			m_PlayerPos = l_Player->GetPosition();
			m_PlayerPos.y = m_PlayerPos.y + Y_STRAFE;
		}

		Vect3f l_Direction;
		l_Direction = GetDirection();
		Vect3f l_Position = m_PlayerPos - l_Direction;
		l_Position.y = l_Position.y + m_LookAtPitch;
		std::vector<Vect3f> l_FrustumCorners = CalculateFrustrumCorners(l_Position, m_ZNear);
		m_NewCameraPosition = CameraCollisionZoom(l_Position, m_PlayerPos, 0.1f, l_FrustumCorners);
	}
}

void CThirdPersonCameraController::SetCamera(CCamera *Camera) const
{
	//CEngine::GetSingleton().GetPhysXManager()->MoveKinematicActor("CamPhysX", Camera->GetPosition());
	Camera->SetFOV(m_Fov);
	Camera->SetAspectRatio(m_AspectRatio);
	Camera->SetPosition(m_NewCameraPosition);
	Camera->SetLookAt(m_PlayerPos);
	Camera->SetUp(GetUp());
	Camera->SetZNear(m_ZNear);
	Camera->SetZFar(m_ZFar);
	Camera->SetMatrixs();
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

void CThirdPersonCameraController::AddRoll(float Radians)
{
	//CCameraController::AddRoll(Radians*m_RollSpeed);
}


Vect3f CThirdPersonCameraController::GetDirection() const
{
	Vect3f l_Direction = CCameraController::GetForward();
	l_Direction.x = m_Zoom*l_Direction.x;
	l_Direction.y = m_Zoom*l_Direction.y;
	l_Direction.z = m_Zoom*l_Direction.z;
	return l_Direction;
}

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

std::vector<Vect3f> CThirdPersonCameraController::CalculateFrustrumCorners(Vect3f CameraPosition, float Distance) const
{
	Vect3f CamForward = CCameraController::GetForward();
	Vect3f CamUp = CCameraController::GetUp();
	Vect3f CamRight = CCameraController::GetRight();
	//Original
	float HalfFOV = m_Fov*0.5f;
	float l_Height = tan(HalfFOV)* Distance;
	float l_Width = l_Height * m_AspectRatio;


	std::vector<Vect3f> l_FrustumCorners;
	//Vect3f Center = CameraPosition + CamForward * Distance; 
	Vect3f Center = CameraPosition - CamForward; //OPCION 2 ESTO ES PARA TENER UN POCO DE MAS ESPACIO Y CALCULAR DETRAS DE LA CAMARA
	l_FrustumCorners.push_back(Center);

	Vect3f BottomRight = Center + (CamRight * l_Width) - (CamUp*l_Height);
	l_FrustumCorners.push_back(BottomRight);

	Vect3f BottomLeft = Center - (CamRight * l_Width) - (CamUp*l_Height);
	l_FrustumCorners.push_back(BottomLeft);

	Vect3f TopRight = Center + (CamRight * l_Width) + (CamUp*l_Height);
	l_FrustumCorners.push_back(TopRight);

	Vect3f TopLeft = Center - (CamRight * l_Width) + (CamUp*l_Height);
	l_FrustumCorners.push_back(TopLeft);

	return l_FrustumCorners;
}

Vect3f CThirdPersonCameraController::CameraCollisionZoom(const Vect3f& camPos, const Vect3f& targetPos, float minOffsetDist, std::vector<Vect3f> frustumNearCorners) const
{
	float l_lengthRayCast;

	Vect3f l_Dist = targetPos - camPos;
	float offsetDist = l_Dist.Length();

	float raycastLength = offsetDist - minOffsetDist;
	if (raycastLength < 0.f)
	{
		// camera is already too near the lookat target
		return camPos;
	}

	Vect3f camOut = targetPos - camPos;
	camOut.Normalize();
	Vect3f nearestCamPos = targetPos - camOut * minOffsetDist;
	float minHitFraction = 1.f;

	for (size_t i = 0; i < frustumNearCorners.size(); i++)
	{
		Vect3f corner = frustumNearCorners[i];
		Vect3f DistanciaEntrePuntos = nearestCamPos - frustumNearCorners[i];
		l_lengthRayCast = DistanciaEntrePuntos.Length();
		/*Vect3f offsetToCorner = corner - camPos;
		Vect3f rayStart = nearestCamPos + offsetToCorner;
		Vect3f rayEnd = corner;*/
		// a result between 0 and 1 indicates a hit along the ray segment

		//CPhysXManager::SRaycastData* RayResult = new CPhysXManager::SRaycastData;

		//bool l_rayCast = CEngine::GetSingleton().GetPhysXManager()->Raycast(rayStart, rayEnd, 0, RayResult);
		//float hitFraction = RayResult->m_Distance;
		//minHitFraction = std::fmin(hitFraction, minHitFraction);
		float l_rayCast = CEngine::GetSingleton().GetPhysXManager()->CameraRaycast(nearestCamPos, corner, l_lengthRayCast);
		float l_rayCastResult = l_rayCast / l_lengthRayCast;

		//float l_rayCast = CEngine::GetSingleton().GetPhysXManager()->Raycast2(rayStart, rayEnd, raycastLength);
		//float l_rayCast = CEngine::GetSingleton().GetPhysXManager()->Raycast2(frustumNearCorners[i], nearestCamPos, l_lengthRayCast);
		//minHitFraction = std::fmin(l_rayCast, minHitFraction);

		minHitFraction = std::fmin(l_rayCastResult, minHitFraction);
	}

	if (minHitFraction < 1.f)
	{
		Vect3f l_NewCamPos = nearestCamPos - camOut * (l_lengthRayCast * minHitFraction); //raycastLength o l_lengthRayCast Cambiado
		return l_NewCamPos;
	}
	else
	{
		return camPos;
	}
}

CEmptyPointerClass* CThirdPersonCameraController::GetZoomLuaAddress() const { return (CEmptyPointerClass *)&m_Zoom; }
CEmptyPointerClass* CThirdPersonCameraController::GetZoomSpeedLuaAddress() const { return (CEmptyPointerClass *)&m_ZoomSpeed; }
CEmptyPointerClass* CThirdPersonCameraController::GetMaxZoomLuaAddress() const { return (CEmptyPointerClass *)&m_MaxZoom; }
CEmptyPointerClass* CThirdPersonCameraController::GetMinZoomLuaAddress() const { return (CEmptyPointerClass *)&m_MinZoom; }

CEmptyPointerClass* CThirdPersonCameraController::GetYawSpeedLuaAddress() const { return (CEmptyPointerClass *)&m_YawSpeed; }
CEmptyPointerClass* CThirdPersonCameraController::GetPitchSpeedLuaAddress() const { return (CEmptyPointerClass *)&m_PitchSpeed; }
CEmptyPointerClass* CThirdPersonCameraController::GetRollSpeedLuaAddress() const { return (CEmptyPointerClass *)&m_RollSpeed; }

CEmptyPointerClass* CThirdPersonCameraController::GetSpeedLuaAddress() const { return (CEmptyPointerClass *)&m_Speed; }
CEmptyPointerClass* CThirdPersonCameraController::GetFastSpeedLuaAddress() const { return (CEmptyPointerClass *)&m_FastSpeed; }

CEmptyPointerClass* CThirdPersonCameraController::GetMinLookAtPitchLuaAddress() const { return (CEmptyPointerClass *)&m_MinLookAtPitch; }
CEmptyPointerClass* CThirdPersonCameraController::GetMaxLookAtPitchLuaAddress() const { return (CEmptyPointerClass *)&m_MaxLookAtPitch; }
