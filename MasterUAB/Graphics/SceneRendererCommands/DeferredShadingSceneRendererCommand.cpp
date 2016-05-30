#include "DeferredShadingSceneRendererCommand.h"
#include "Engine.h"
#include "RenderableObjects\RenderableObjectTechniqueManager.h"
#include "Render\RenderManager.h"
#include "Lights\LightManager.h"

CDeferredShadingSceneRendererCommand:: CDeferredShadingSceneRendererCommand(CXMLTreeNode &TreeNode): CStagedTexturedSceneRendererCommand(TreeNode)
{
	D3D11_BLEND_DESC l_AlphablendDesc;
	ZeroMemory(&l_AlphablendDesc, sizeof(D3D11_BLEND_DESC));
	l_AlphablendDesc.RenderTarget[0].BlendEnable=true;
	l_AlphablendDesc.RenderTarget[0].SrcBlend=D3D11_BLEND_ONE;
	l_AlphablendDesc.RenderTarget[0].DestBlend=D3D11_BLEND_ONE;
	l_AlphablendDesc.RenderTarget[0].BlendOp=D3D11_BLEND_OP_ADD;
	l_AlphablendDesc.RenderTarget[0].SrcBlendAlpha=D3D11_BLEND_ZERO;
	l_AlphablendDesc.RenderTarget[0].DestBlendAlpha=D3D11_BLEND_ZERO;
	l_AlphablendDesc.RenderTarget[0].BlendOpAlpha=D3D11_BLEND_OP_ADD;
	l_AlphablendDesc.RenderTarget[0].RenderTargetWriteMask=D3D11_COLOR_WRITE_ENABLE_ALL;

	if(FAILED(CEngine::GetSingleton().GetRenderManager()->GetContextManager()->GetDevice()->CreateBlendState(&l_AlphablendDesc, &m_EnabledAlphaBlendState)))
		return;
	m_RenderableObjectTechnique=CEngine::GetSingleton().GetRenderableObjectTechniqueManager()->GetResource("MV_POSITION4_COLOR_TEXTURE_VERTEX");
} 

CDeferredShadingSceneRendererCommand::~CDeferredShadingSceneRendererCommand()
{
	m_EnabledAlphaBlendState->Release();
	m_EnabledAlphaBlendState = 0;
}

void CDeferredShadingSceneRendererCommand::Execute(CRenderManager &RenderManager)
{
	CEngine::GetSingleton().GetRenderManager()->GetContextManager()->SetAlphaBlendState(m_EnabledAlphaBlendState);

	CLightManager* l_LightManager = CEngine::GetSingleton().GetLightManager();
	std::vector<CLight*> l_Lights = l_LightManager->GetResourcesVector();
	size_t l_Size = l_Lights.size();

	int l_Width = RenderManager.GetContextManager()->GetFrameBufferWidth();
	int l_Height = RenderManager.GetContextManager()->GetFrameBufferHeight();

	ActivateTextures();

	//CEngine::GetSingleton().GetEffectManager()->SetLightsConstantsDefaultValues();

	/*Diffuse + Specular*/
	for(size_t i=0;i<l_Size;++i)
	{
		/*Only 1 light*/
		if(l_Lights[i]->GetActive())
		{
			CEngine::GetSingleton().GetEffectManager()->SetLightConstants(0,l_Lights[i]);
			m_RenderableObjectTechnique->GetEffectTechnique()->SetConstantBuffer(1, &CEffectManager::m_LightEffectParameters);
			RenderManager.DrawScreenQuad(m_RenderableObjectTechnique->GetEffectTechnique(),NULL,0,0,1.0f,1.0f,CColor(v4fZERO));
		}
	}

	CEngine::GetSingleton().GetRenderManager()->GetContextManager()->DisableAlphaBlendState();
} 
