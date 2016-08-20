#include "Lights\SpotLight.h"
#include "Textures\DynamicTexture.h"
#include "XML\XMLTreeNode.h"
#include "Render\RenderManager.h"

CSpotLight::CSpotLight(const std::string &Name)
:m_Angle(0.0f)
,m_FallOff(0.0f)
,CDirectionalLight(Name)
{
	m_Type = SPOT;
}

CSpotLight::CSpotLight(CXMLTreeNode &TreeNode) 
:CDirectionalLight(TreeNode)
,m_FallOff(TreeNode.GetFloatProperty("fall_off", 0.0f))
,m_Angle(TreeNode.GetFloatProperty("angle", 0.0f))
{
	m_Type = SPOT;
}

void CSpotLight::Render(CRenderManager *RenderManager)
{

}

void CSpotLight::SetShadowMap(CRenderManager &RenderManager)
{
	if (m_ShadowMap == NULL)
	{
		assert(false);
		return;
	}

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

void CSpotLight::SetBlackAndWhiteMap(CRenderManager &RenderManager, bool Blurred)
{
	CDynamicTexture* l_Texture = m_BlackAndWhiteMap;
	if (Blurred)
		l_Texture = m_BlackAndWhiteBlurredMap;

	if (l_Texture == NULL)
	{
		assert(false);
		return;
	}

	unsigned int l_ShadowMapWidth = l_Texture->GetWidth();
	unsigned int l_ShadowMapHeight = l_Texture->GetHeight();
	
	ID3D11RenderTargetView *l_RenderTargetViews[1];
	l_RenderTargetViews[0] = l_Texture->GetRenderTargetView();
	D3D11_VIEWPORT m_viewport;
	m_viewport.Width = (float)l_ShadowMapWidth;
	m_viewport.Height = (float)l_ShadowMapHeight;
	m_viewport.MinDepth = 0.0f;
	m_viewport.MaxDepth = 1.0f;
	m_viewport.TopLeftX = 0.0f;
	m_viewport.TopLeftY = 0.0f;
	RenderManager.GetContextManager()->GetDeviceContext()->RSSetViewports(1, &m_viewport);
	RenderManager.GetContextManager()->SetRenderTargets(1, l_RenderTargetViews, l_Texture->GetDepthStencilView());
}

CEmptyPointerClass* CSpotLight::GetFallOffLuaAddress() const  { return (CEmptyPointerClass *)((void*)&m_FallOff); }
CEmptyPointerClass* CSpotLight::GetAngleLuaAddress() const  { return (CEmptyPointerClass *)((void*)&m_Angle); }
