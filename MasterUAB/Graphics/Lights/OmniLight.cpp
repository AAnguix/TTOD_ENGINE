#include "Lights\OmniLight.h"

COmniLight::COmniLight(const std::string &Name)
:CLight(Name)
{
	m_Type = OMNI;
	m_GenerateShadowMap = false;
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

void COmniLight::SetBlackAndWhiteMap(CRenderManager &RenderManager)
{
}
