#ifndef _CAMERAINFO_H
#define _CAMERAINFO_H

class CXMLTreeNode;
#include "Math\Vector3.h"

class CCameraInfo 
{ 
	public: 
		float m_NearPlane, m_FarPlane;
		float m_FOV; 
		Vect3f m_Eye, m_LookAt; 
		Vect3f m_Up; 

		//new 
		Vect3f m_Position;

		CCameraInfo(); 
		CCameraInfo(const Vect3f Position, const Vect3f &Eye, const Vect3f &LookAt, const Vect3f &Up, float NearPlane, float FarPlane, float FOV); 
		CCameraInfo(CXMLTreeNode &XMLTreeNode);
};

#endif