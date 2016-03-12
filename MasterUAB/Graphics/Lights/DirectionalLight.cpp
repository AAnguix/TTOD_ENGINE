#include "Lights\DirectionalLight.h"
#include "Render\RenderManager.h"
#include "Textures\DynamicTexture.h"

CDirectionalLight::CDirectionalLight():CLight(),
	m_Direction(v3fZERO)
{
	SetType(DIRECTIONAL);
}

CDirectionalLight::CDirectionalLight(CXMLTreeNode &TreeNode):CLight(TreeNode)
{
	SetType(DIRECTIONAL);
	SetDirection(TreeNode.GetVect3fProperty("dir",v3fZERO));
	m_OrthoShadowMapSize = TreeNode.GetVect2fProperty("ortho_shadow_map_size",v2fZERO);
}

void CDirectionalLight::Render(CRenderManager *RM)
{

}

void CDirectionalLight::SetShadowMap(CRenderManager &RenderManager)
{
	m_ViewShadowMap.SetIdentity();
	m_ViewShadowMap.SetFromLookAt(m_Position, m_Position+m_Direction, v3fY);
	
	unsigned int l_ShadowMapWidth=m_ShadowMap->GetWidth();
	unsigned int l_ShadowMapHeight=m_ShadowMap->GetHeight();
	
	m_ProjectionShadowMap.SetFromOrtho(m_OrthoShadowMapSize.x,m_OrthoShadowMapSize.y, 0.1f, m_EndRangeAttenuation);
	
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