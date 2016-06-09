#include "Lights\OmniLight.h"

COmniLight::COmniLight() 
:CLight()
{
	m_Type = OMNI;
}

COmniLight::COmniLight(CXMLTreeNode &TreeNode) 
:CLight(TreeNode)
{
	m_Type = OMNI;
	m_GenerateShadowMap=false;
}

void COmniLight::Render(CRenderManager *RenderManager)
{
	
}

void COmniLight::SetShadowMap(CRenderManager &RenderManager)
{

}
