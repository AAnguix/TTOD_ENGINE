#include "Render\Frustum.h"

CFrustum::CFrustum() 
{
	for (int i = 0; i < 6; ++i)
	{
		m_planes[i] = { 0.0f, 0.0f, 0.0f, 0.0f };
	}
	
}
void CFrustum::Update(const float screenDepth, const Mat44f INprojectionMatrix, const Mat44f INviewMatrix)
{
	DirectX::XMFLOAT4X4 projectionMatrix = ConvertMat44fToDxXMF4X4(INprojectionMatrix);
	DirectX::XMFLOAT4X4 viewMatrix = ConvertMat44fToDxXMF4X4(INviewMatrix);
	DirectX::XMMATRIX SwapProjectionMatrix = DirectX::XMLoadFloat4x4(&projectionMatrix);
	DirectX::XMMATRIX SwapViewMatrix = DirectX::XMLoadFloat4x4(&viewMatrix);

	float zMinimum, r;
	DirectX::XMFLOAT4X4  matrix;
	DirectX::XMMATRIX  result_matrix;
	
	DirectX::XMVECTOR SwapVector = DirectX::XMVECTOR( DirectX::XMVectorZero() );

	// Calculate the minimum Z distance in the frustum.
	zMinimum = -projectionMatrix._43 / projectionMatrix._33;
	r = screenDepth / (screenDepth - zMinimum);
	projectionMatrix._33 = r;
	projectionMatrix._43 = -r * zMinimum;

	// Create the frustum matrix from the view matrix and updated projection matrix.
	result_matrix = DirectX::XMMatrixMultiply(SwapViewMatrix, SwapProjectionMatrix);

	DirectX::XMStoreFloat4x4(&matrix, result_matrix);

	// Calculate near plane of frustum.
	m_planes[0].x = matrix._14 + matrix._13;
	m_planes[0].y = matrix._24 + matrix._23;
	m_planes[0].z = matrix._34 + matrix._33;
	m_planes[0].w = matrix._44 + matrix._43;
	SwapVector = XMLoadFloat4(&m_planes[0]);
	DirectX::XMPlaneNormalize(SwapVector);
	DirectX::XMStoreFloat4(&m_planes[0], SwapVector);

	// Calculate far plane of frustum.
	m_planes[1].x = matrix._14 - matrix._13;
	m_planes[1].y = matrix._24 - matrix._23;
	m_planes[1].z = matrix._34 - matrix._33;
	m_planes[1].w = matrix._44 - matrix._43;
	SwapVector = XMLoadFloat4(&m_planes[1]);
	DirectX::XMPlaneNormalize(SwapVector);
	DirectX::XMStoreFloat4(&m_planes[1], SwapVector);

	// Calculate left plane of frustum.
	m_planes[2].x = matrix._14 + matrix._11;
	m_planes[2].y = matrix._24 + matrix._21;
	m_planes[2].z = matrix._34 + matrix._31;
	m_planes[2].w = matrix._44 + matrix._41;
	SwapVector = XMLoadFloat4(&m_planes[2]);
	DirectX::XMPlaneNormalize(SwapVector);
	DirectX::XMStoreFloat4(&m_planes[2], SwapVector);

	// Calculate right plane of frustum.
	m_planes[3].x = matrix._14 - matrix._11;
	m_planes[3].y = matrix._24 - matrix._21;
	m_planes[3].z = matrix._34 - matrix._31;
	m_planes[3].w = matrix._44 - matrix._41;
	SwapVector = XMLoadFloat4(&m_planes[3]);
	DirectX::XMPlaneNormalize(SwapVector);
	DirectX::XMStoreFloat4(&m_planes[3], SwapVector);

	// Calculate top plane of frustum.
	m_planes[4].x = matrix._14 - matrix._12;
	m_planes[4].y = matrix._24 - matrix._22;
	m_planes[4].z = matrix._34 - matrix._32;
	m_planes[4].w = matrix._44 - matrix._42;
	SwapVector = XMLoadFloat4(&m_planes[4]);
	DirectX::XMPlaneNormalize(SwapVector);
	DirectX::XMStoreFloat4(&m_planes[4], SwapVector);

	// Calculate bottom plane of frustum.
	m_planes[5].x = matrix._14 + matrix._12;
	m_planes[5].y = matrix._24 + matrix._22;
	m_planes[5].z = matrix._34 + matrix._32;
	m_planes[5].w = matrix._44 + matrix._42;
	SwapVector = XMLoadFloat4(&m_planes[5]);
	DirectX::XMPlaneNormalize(SwapVector);
	DirectX::XMStoreFloat4(&m_planes[5], SwapVector);

	return;
}

bool CFrustum::CheckPoint(Vect3f Point)
{
	
	// Check if the point is inside all six planes of the view frustum.
	for (size_t i = 0; i<6; ++i)
	{
		
		if (PlaneDotCoord(m_planes[i], Point) < 0.0f)
		{
			return false;
		}
	}

	return true;
}

bool CFrustum::CheckCube(Vect3f Point, float radius)
{
	// Check if any one point of the cube is in the view frustum.
	for (size_t i = 0; i<6; i++)
	{
		if (PlaneDotCoord(m_planes[i], Vect3f((Point.x - radius), (Point.y - radius), (Point.z - radius))) >= 0.0f)
		{
			continue;
		}
		if (PlaneDotCoord(m_planes[i], Vect3f((Point.x + radius), (Point.y - radius), (Point.z - radius))) >= 0.0f)
		{
			continue;
		}
		if (PlaneDotCoord(m_planes[i], Vect3f((Point.x - radius), (Point.y + radius), (Point.z - radius))) >= 0.0f)
		{
			continue;
		}
		if (PlaneDotCoord(m_planes[i], Vect3f((Point.x + radius), (Point.y + radius), (Point.z - radius))) >= 0.0f)
		{
			continue;
		}
		if (PlaneDotCoord(m_planes[i], Vect3f((Point.x - radius), (Point.y - radius), (Point.z + radius))) >= 0.0f)
		{
			continue;
		}
		if (PlaneDotCoord(m_planes[i], Vect3f((Point.x + radius), (Point.y - radius), (Point.z + radius))) >= 0.0f)
		{
			continue;
		}
		if (PlaneDotCoord(m_planes[i], Vect3f((Point.x - radius), (Point.y + radius), (Point.z + radius))) >= 0.0f)
		{
			continue;
		}
		if (PlaneDotCoord(m_planes[i], Vect3f((Point.x + radius), (Point.y + radius), (Point.z + radius))) >= 0.0f)
		{
			continue;
		}

		return false;
	}

	return true;
}

bool CFrustum::CheckSphere(Vect3f Point, float radius)
{
	// Check if the radius of the sphere is inside the view frustum.
	for (size_t i = 0; i<6; i++)
	{
		if (PlaneDotCoord(m_planes[i], Point) < -radius)
		{
			return false;
		}
	}

	return true;

}

DirectX::XMFLOAT4X4 CFrustum::ConvertMat44fToDxXMF4X4(const Mat44f Matrix)
{
	DirectX::XMFLOAT4X4 l_DirectxMatrix;

	l_DirectxMatrix._11 = Matrix.m00;
	l_DirectxMatrix._12 = Matrix.m01;
	l_DirectxMatrix._13 = Matrix.m02;
	l_DirectxMatrix._14 = Matrix.m03;

	l_DirectxMatrix._21 = Matrix.m10;
	l_DirectxMatrix._22 = Matrix.m11;
	l_DirectxMatrix._23 = Matrix.m12;
	l_DirectxMatrix._24 = Matrix.m13;

	l_DirectxMatrix._31 = Matrix.m20;
	l_DirectxMatrix._32 = Matrix.m21;
	l_DirectxMatrix._33 = Matrix.m22;
	l_DirectxMatrix._34 = Matrix.m23;

	l_DirectxMatrix._41 = Matrix.m30;
	l_DirectxMatrix._42 = Matrix.m31;
	l_DirectxMatrix._43 = Matrix.m32;
	l_DirectxMatrix._44 = Matrix.m33;

	return l_DirectxMatrix;
}

float CFrustum::PlaneDotCoord(DirectX::XMFLOAT4  Plane, Vect3f Point)
{
	float Result = Plane.x*Point.x + Plane.y*Point.y + Plane.z*Point.z + Plane.w*1.0f;
	return Result;
}