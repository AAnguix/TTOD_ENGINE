#ifndef _CAMERAKEYCONTROLLER_H
#define _CAMERAKEYCONTROLLER_H

#include "Cinematics\CameraKey.h"
class XMLTreeNode;
class CCamera;
#include <vector>
#include "Camera\CameraController.h"

class CCameraKeyController : public CCameraController
{ 

private: 
	std::vector<CCameraKey *> m_Keys; 
	size_t m_CurrentKey, m_NextKey;
	float m_CurrentTime, m_TotalTime;
	bool m_Cycle; 
	bool m_Reverse;
	bool m_Backing; //Own

	Vect3f m_NewPosition;
	float m_NewFOV;
	float m_NewNearPlane;
	float m_NewFarPlane;
	Vect3f m_NewLookAt;

	void LoadXML(const std::string &FileName); 
	void GetCurrentKey(); 
	
public: 
	CCameraKeyController(CXMLTreeNode &XMLTreeNode); 
	~CCameraKeyController(); 
	void SetCamera(CCamera *Camera) const;
	void Update(float ElapsedTime); 
	void SetCurrentTime(float CurrentTime); 
	void ResetTime(); 
	float GetTotalTime(); 
	bool IsCycle() const; 
	void SetCycle(bool Cycle); 
	bool IsReverse() const; 
	void SetReverse(bool Reverse); 

	EType GetType() const;
 };

#endif