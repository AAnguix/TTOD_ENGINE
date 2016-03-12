#ifndef _OMNILIGHT_H
#define _OMNILIGHT_H

#include "Lights\Light.h"

class COmniLight : public CLight 
{ 
public:  
	COmniLight();
	COmniLight(CXMLTreeNode &TreeNode);
	void Render(CRenderManager *RenderManager); 
	void SetShadowMap(CRenderManager &RenderManager);
};  
 
#endif