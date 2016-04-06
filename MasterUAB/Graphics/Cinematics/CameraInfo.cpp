#include "CameraInfo.h"
#include "XML\XMLTreeNode.h"

CCameraInfo::CCameraInfo() : m_Position(v3fZERO),m_Eye(v3fZERO),m_LookAt(v3fZERO),m_Up(v3fZERO),m_NearPlane(0.0f),m_FarPlane(0.0f),m_FOV(0.0f)
{
	
	
} 

CCameraInfo::CCameraInfo(const Vect3f Position, const Vect3f &Eye, const Vect3f &LookAt, const Vect3f &Up, float NearPlane, float FarPlane, float FOV)
: m_Eye(Eye)
,m_LookAt(LookAt)
,m_Up(Up)
,m_NearPlane(NearPlane)
,m_FarPlane(FarPlane)
,m_FOV(FOV)
,m_Position(Position)
{
	
}
 
CCameraInfo::CCameraInfo(CXMLTreeNode &XMLTreeNode) :
m_LookAt(XMLTreeNode.GetVect3fProperty("look_at", v3fZERO))
,m_FOV(XMLTreeNode.GetFloatProperty("fov", 0.0f))
,m_NearPlane(XMLTreeNode.GetFloatProperty("near_plane", 0.0f))
,m_FarPlane(XMLTreeNode.GetFloatProperty("far_plane", 0.0f))
,m_Position(XMLTreeNode.GetVect3fProperty("pos", v3fZERO))
,m_Up(Vect3f(0.0f, 1.0f, 0.0f))
{
	//m_Up=XMLTreeNode.GetVect3fProperty("up",Vect3f(0.0f,1.0f,0.0f));	
}