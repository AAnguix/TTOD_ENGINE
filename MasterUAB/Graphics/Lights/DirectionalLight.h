#ifndef _DIRECTIONALLIGHT_H
#define _DIRECTIONALLIGHT_H

#include "Lights\Light.h"
#include "Utils\EmptyPointerClass.h"

class CDirectionalLight : public CLight 
{ 

protected:  
	Vect3f m_Direction; 
	Vect2f m_OrthoShadowMapSize; 
	Vect3f m_PlayerOffset;

public:  
	CDirectionalLight(const std::string &Name);
	CDirectionalLight(CXMLTreeNode &TreeNode);
	UAB_GET_SET_PROPERTY_REFERENCE(Vect3f,Direction);
	virtual void Render(CRenderManager *RM); 
	void SetShadowMap(CRenderManager &RenderManager);
	void SetBlackAndWhiteMap(CRenderManager &RenderManager, bool Blurred);
	UAB_GET_VAR_ADDRESS(Direction);
}; 

#endif