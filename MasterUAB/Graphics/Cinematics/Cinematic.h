#ifndef _CINEMATIC_H
#define _CINEMATIC_H

#include "RenderableObjects\RenderableObject.h"
#include "Cinematics\CinematicPlayer.h"
#include "Cinematics\CinematicObject.h"
#include <vector>

class CCinematic : public CRenderableObject, public CCinematicPlayer 
{

private:
	std::string m_FileName; 

protected: 
	std::vector<CCinematicObject *> m_CinematicObjects; 

public: 
	CCinematic(CXMLTreeNode &TreeNode); 
	virtual ~CCinematic(); 
	virtual void Stop(); 
	virtual void Play(bool Cycle); 
	virtual void Pause(); 
	void LoadXML(const std::string &Filename); 
	void AddCinematicObject(CCinematicObject *CinematicObject); 
	void Update(float ElapsedTime); 
	virtual void Render(CRenderManager *RM); 
	TRenderableObjectType GetClassType() const{ return TRenderableObjectType::CINEMATIC; };
};

#endif