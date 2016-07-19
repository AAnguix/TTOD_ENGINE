#ifndef _FRUSTUM_H
#define _FRUSTUM_H

#include "Math\Matrix44.h"
#include "Math\Vector3.h"
#include <DirectXMath.h>

class CFrustum 
{
private:
	DirectX::XMFLOAT4  m_planes[6];

public:
	CFrustum();
	void Update(const float screenDepth, const Mat44f projectionMatrix, const Mat44f viewMatrix);
	bool CheckPoint(Vect3f Point);
	bool CheckCube(Vect3f Point, float radius);
	bool CheckSphere(Vect3f Point, float radius);
	DirectX::XMFLOAT4X4 ConvertMat44fToDxXMF4X4(const Mat44f Matrix);
	float PlaneDotCoord(DirectX::XMFLOAT4  Plane, Vect3f Point);
};

#endif
