#ifndef _THIRDPERSONCAMERACONTROLLER_H
#define _THIRDPERSONCAMERACONTROLLER_H

#include "Camera\CameraController.h"
#include <vector>
class CCamera;
class CXMLTreeNode;


class CThirdPersonCameraController : public CCameraController
{
private:
	float m_YawSpeed, m_PitchSpeed, m_RollSpeed;
	float m_Speed, m_FastSpeed;
	
	float m_Zoom, m_ZoomSpeed, m_MaxZoom, m_MinZoom;
	
	float m_LookAtPitch, m_MaxLookAtPitch, m_MinLookAtPitch;

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
	Vect3f GetDirection() const;
	void Update(float ElapsedTime);

	//void Update(Vect3f rotation);

	float GetPlayerCameraAngleDif(Vect3f PlayerPosition);

	EType GetType() const{ return THIRD_PERSON; };

};

#endif
