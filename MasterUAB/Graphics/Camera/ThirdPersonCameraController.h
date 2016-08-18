#ifndef _THIRDPERSONCAMERACONTROLLER_H
#define _THIRDPERSONCAMERACONTROLLER_H

#include "Camera\CameraController.h"
#include <vector>
#include "CameraSmoother.h"

class CCamera;
class CXMLTreeNode;
class CEmptyPointerClass;
class CCameraShaker;

class CThirdPersonCameraController : public CCameraController
{
private:
	float m_YawSpeed, m_PitchSpeed, m_RollSpeed;
	float m_Speed, m_FastSpeed;
	
	float m_Zoom, m_ZoomSpeed, m_MaxZoom, m_MinZoom;
	
	float m_LookAtPitch, m_MaxLookAtPitch, m_MinLookAtPitch;

	CCameraSmoother m_Smoother;
	CCameraShaker* m_Shaker;
	bool m_Shaking;

	Vect3f m_PlayerPos;
	float m_CurrentTime;
	Vect3f m_NewCameraPosition;
	std::vector<Vect3f> CalculateFrustrumCorners(Vect3f CameraPosition, float Distance) const;
	Vect3f CameraCollisionZoom(const Vect3f& camPos, const Vect3f& targetPos, float minOffsetDist, std::vector<Vect3f> frustumNearCorners) const;

public:
	CThirdPersonCameraController();
	CThirdPersonCameraController(CXMLTreeNode &TreeNode);
	virtual ~CThirdPersonCameraController();

	void AddLookAtPitch(float Pitch);
	
	void AddZoom(float Zoom);
	void SetZoom(float Zoom) { m_Zoom = Zoom; }
	
	void Move(float Strafe, float Forward, bool Speed, float ElapsedTime);
	void MoveUpDown(float Movement, bool Speed, float ElapsedTime);
	void SetCamera(CCamera *Camera) const;

	void AddYaw(float Radians);
	void AddPitch(float Radians);
	void AddRoll(float Radians);

	Vect3f GetDirection() const;
	void Update(float ElapsedTime);

	float GetPlayerCameraAngleDif(Vect3f PlayerPosition);
	EType GetType() const{ return THIRD_PERSON; };

	void StartSmoothing();
	void StopSmoothing();
	void StartShaking(Vect3f CameraRotation, int Duration, int YawFrequency, float MinYawRandom, float MaxYawRandom, int PitchFrequency, float MinPitchRandom, float MaxPitchRandom);

	CEmptyPointerClass* GetZoomLuaAddress() const;
	CEmptyPointerClass* GetZoomSpeedLuaAddress() const;
	CEmptyPointerClass* GetMinZoomLuaAddress() const;
	CEmptyPointerClass* GetMaxZoomLuaAddress() const;
	CEmptyPointerClass* GetYawSpeedLuaAddress() const;
	CEmptyPointerClass* GetPitchSpeedLuaAddress() const;
	CEmptyPointerClass* GetRollSpeedLuaAddress() const;
	CEmptyPointerClass* GetSpeedLuaAddress() const;
	CEmptyPointerClass* GetFastSpeedLuaAddress() const;
	CEmptyPointerClass* GetMinLookAtPitchLuaAddress() const;
	CEmptyPointerClass* GetMaxLookAtPitchLuaAddress() const;
};

#endif
