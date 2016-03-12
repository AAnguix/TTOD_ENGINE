#include "Lights\OmniLight.h"

COmniLight::COmniLight() : CLight()
{
	SetType(OMNI);
}

COmniLight::COmniLight(CXMLTreeNode &TreeNode) : CLight(TreeNode)
{
	SetType(OMNI);
	SetGenerateShadowMap(false);
}

void COmniLight::Render(CRenderManager *RenderManager)
{

}

void COmniLight::SetShadowMap(CRenderManager &RenderManager)
{

}
