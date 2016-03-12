#include "Lights\SpotLight.h"
#include "Textures\DynamicTexture.h"

CSpotLight::CSpotLight() : m_Angle(0),m_FallOff(0),CDirectionalLight()
{
	SetType(SPOT);
}

CSpotLight::CSpotLight(CXMLTreeNode &TreeNode) : CDirectionalLight(TreeNode)
{
	SetType(SPOT);
	SetFallOff(TreeNode.GetFloatProperty("fall_off",0.0f));
	SetAngle(TreeNode.GetFloatProperty("angle",0.0f));
}

void CSpotLight::Render(CRenderManager *RenderManager)
{

}

void CSpotLight::SetShadowMap(CRenderManager &RenderManager)
{
	if(m_ShadowMap==NULL)
		return;
	
	m_ViewShadowMap.SetIdentity();
	m_ViewShadowMap.SetFromLookAt(m_Position, m_Position+m_Direction, v3fY);
	unsigned int l_ShadowMapWidth=m_ShadowMap->GetWidth();
	unsigned int l_ShadowMapHeight=m_ShadowMap->GetHeight();
	m_ProjectionShadowMap.SetIdentity();
	m_ProjectionShadowMap.SetFromPerspective(m_FallOff,
	l_ShadowMapWidth/(float)l_ShadowMapHeight, 0.1f, m_EndRangeAttenuation);
	CEffectManager::m_SceneEffectParameters.m_View=m_ViewShadowMap;
	CEffectManager::m_SceneEffectParameters.m_Projection=m_ProjectionShadowMap;
	ID3D11RenderTargetView *l_RenderTargetViews[1];
	l_RenderTargetViews[0]=m_ShadowMap->GetRenderTargetView();
	D3D11_VIEWPORT m_viewport;
	m_viewport.Width=(float)l_ShadowMapWidth;
	m_viewport.Height=(float)l_ShadowMapHeight;
	m_viewport.MinDepth=0.0f;
	m_viewport.MaxDepth=1.0f;
	m_viewport.TopLeftX=0.0f;
	m_viewport.TopLeftY=0.0f;
	RenderManager.GetContextManager()->GetDeviceContext()->RSSetViewports(1, &m_viewport);
	RenderManager.GetContextManager()->SetRenderTargets(1, l_RenderTargetViews, m_ShadowMap->GetDepthStencilView());
}

