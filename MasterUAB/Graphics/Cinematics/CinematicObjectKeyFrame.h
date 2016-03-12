#ifndef _CINEMATICOBJECTKEYFRAME_H
#define _CINEMATICOBJECTKEYFRAME_H

#include "Utils\3DElement.h"
class XMLTreeNode;

class CCinematicObjectKeyFrame : public C3DElement 
{ 

private: 
	float m_KeyFrameTime; 

public: 
	CCinematicObjectKeyFrame(CXMLTreeNode &XMLTreeNode); 
	float GetKeyFrameTime() const; 
	void SetKeyFrameTime(const float KeyFrameTime); 
};

#endif