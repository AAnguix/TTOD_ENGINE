#ifndef _SPOTLIGHT_H
#define _SPOTLIGHT_H

#include "Lights\DirectionalLight.h"

class CSpotLight : public CDirectionalLight 
{  

protected:  
	float	m_Angle; 
	float	m_FallOff; 

public:   
	CSpotLight();
	CSpotLight(CXMLTreeNode &TreeNode); 
	UAB_GET_SET_STANDARD_PROPERTY(float, Angle);  
	UAB_GET_SET_STANDARD_PROPERTY(float, FallOff);
	void Render(CRenderManager *RenderManager); 
	void SetShadowMap(CRenderManager &RenderManager);
	UAB_GET_VAR_ADDRESS(Angle);
	UAB_GET_VAR_ADDRESS(FallOff);
};

#endif