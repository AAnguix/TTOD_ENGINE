#ifndef _FPSCAMERACONTROLLER_H
#define _FPSCAMERACONTROLLER_H

#include "Camera\CameraController.h"
class CCamera;
class CXMLTreeNode;

class CFPSCameraController : public CCameraController
{
private:
	float						m_YawSpeed;
	float						m_PitchSpeed;
	float						m_RollSpeed;
	float						m_Speed;
	float						m_FastSpeed;
public:
	CFPSCameraController();
	CFPSCameraController(CXMLTreeNode &TreeNode);
	virtual ~CFPSCameraController();
	
	void Move(float Strafe, float Forward, bool Speed, float ElapsedTime);
	void MoveUpDown(float Movement, bool Speed, float ElapsedTime);
	void SetCamera(CCamera *Camera) const;
	void AddYaw(float Radians);
	void AddPitch(float Radians);
	Vect3f GetDirection() const;

	EType GetType() const;

};

#endif
