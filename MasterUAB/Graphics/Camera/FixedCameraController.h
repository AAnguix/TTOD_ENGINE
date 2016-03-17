#ifndef _FIXEDCAMERACONTROLLER_H
#define _FIXEDCAMERACONTROLLER_H

#include "Camera\CameraController.h"
class CXMLTreeNode;

class CFixedCameraController : public CCameraController
{
private:
	Vect3f m_LookAt;
	
public:
	CFixedCameraController();
	CFixedCameraController(CXMLTreeNode &TreeNode);
	virtual ~CFixedCameraController();

	void SetCamera(CCamera *Camera) const;
	Vect3f GetDirection() const;
};

#endif