#ifndef _CINEMATICOBJECT_H
#define _CINEMATICOBJECT_H

#include "Cinematics\CinematicPlayer.h"
#include "Cinematics\CinematicObjectKeyFrame.h"
#include <vector>
class CXMLTreeNode;
class CRenderableObject;

class CCinematicObject : public CCinematicPlayer 
{ 

private: 
	std::vector<CCinematicObjectKeyFrame *> m_CinematicObjectKeyFrames; 
	size_t m_CurrentKeyFrame; 
	CRenderableObject *m_RenderableObject; 

	Quatf Slerp(Quatf q0, Quatf q1,float t);
	Vect3f CCinematicObject::ObjectToInertialQuaternion(Quatf q);

public: 
	CCinematicObject(CXMLTreeNode &TreeNode); 
	bool IsOk(); 
	virtual ~CCinematicObject(); 
	void AddCinematicObjectKeyFrame(CCinematicObjectKeyFrame *CinematicObjectKeyFrame); 
	void Update(float ElapsedTime); 
	void Stop(); 
	void OnRestartCycle(); 
	size_t GetCurrentKeyFrame() const{ return m_CurrentKeyFrame; };
	void SetCurrentKeyFrame(size_t KeyFrame) { m_CurrentKeyFrame = KeyFrame; };
	CRenderableObject* GetRenderableObject() const{ return m_RenderableObject; };
	std::vector<CCinematicObjectKeyFrame *> GetCinematicObjectKeyFrames() const{ return m_CinematicObjectKeyFrames; };
};

#endif