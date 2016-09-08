#include "Camera\Camera.h"
#include "XML\XMLTreeNode.h"

CCamera::CCamera()
:m_FOV(0.0f) /*60.0f*/
,m_AspectRatio(0.0f)
,m_ZNear(0.0f)
,m_ZFar(0.0f)
,m_Position(5.0f, 5.0f, 0.0f)
,m_LookAt(0.0f, 0.0f, 0.0f)
,m_Up(0.0f, 1.0f, 0.0f)
,CNamed("DefaultCamera")
{
}

CCamera::CCamera(const std::string &Name)
:m_FOV(0.0f)
,m_AspectRatio(0.0f)
,m_ZNear(0.0f)
,m_ZFar(0.0f)
,m_Position(5.0f, 5.0f, 0.0f)
,m_LookAt(0.0f, 0.0f, 0.0f)
,m_Up(0.0f, 1.0f, 0.0f)
,CNamed(Name)
{
}

CCamera::~CCamera(){}

Vect2f CCamera::GetPositionInScreenCoordinates(const Vect3f &Position) const
{
	Mat44f l_ViewProj=m_View*m_Projection;
	Vect4f l_Pos4f(Position.x, Position.y, Position.z, 1.0);
	float x = l_Pos4f.x*l_ViewProj.m00 + l_Pos4f.y*l_ViewProj.m10 +
	l_Pos4f.z*l_ViewProj.m20 + l_ViewProj.m30;
	float y = l_Pos4f.x*l_ViewProj.m01 + l_Pos4f.y*l_ViewProj.m11 +
	l_Pos4f.z*l_ViewProj.m21 + l_ViewProj.m31;
	float z = l_Pos4f.x*l_ViewProj.m02 + l_Pos4f.y*l_ViewProj.m12 +
	l_Pos4f.z*l_ViewProj.m22 + l_ViewProj.m32;
	float w = l_Pos4f.x*l_ViewProj.m03 + l_Pos4f.y*l_ViewProj.m13 +
	l_Pos4f.z*l_ViewProj.m23 + l_ViewProj.m33;
	
	if(w==0)
		w=1;
	return Vect2f(x/w, y/w);
}