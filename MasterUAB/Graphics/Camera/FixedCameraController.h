#ifndef _FIXEDCAMERACONTROLLER_H
#define _FIXEDCAMERACONTROLLER_H

#include "Camera\CameraController.h"
class CXMLTreeNode;

class CFixedCameraController : public CCameraController
{
private:
	Vect3f m_LookAt;
	float m_FOV;
	float m_Near;
	float m_Far;
	
public:
	CFixedCameraController();
	CFixedCameraController(CXMLTreeNode &TreeNode);
	virtual ~CFixedCameraController();

	void SetCamera(CCamera *Camera) const;
	Vect3f GetDirection() const;
};

#endif