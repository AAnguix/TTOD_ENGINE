#include "DeferredShadingSceneRendererCommand.h"
#include "Engine.h"
#include "RenderableObjects\RenderableObjectTechniqueManager.h"
#include "Render\RenderManager.h"
#include "Lights\LightManager.h"
#include "StaticMeshes\StaticMeshManager.h"
#include "Render\ContextManager.h"
#include "Lights\OmniLight.h"
#include "Utils\3DElement.h"

CDeferredShadingSceneRendererCommand:: CDeferredShadingSceneRendererCommand(CXMLTreeNode &TreeNode): CStagedTexturedSceneRendererCommand(TreeNode)
{
	m_UseLightVolumes = true;
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

	m_Sphere = CEngine::GetSingleton().GetStaticMeshManager()->GetResource("deferred_shading_sphere");
} 

CDeferredShadingSceneRendererCommand::~CDeferredShadingSceneRendererCommand()
{
	m_EnabledAlphaBlendState->Release();
	m_EnabledAlphaBlendState = 0;
}

void CDeferredShadingSceneRendererCommand::Execute(CRenderManager &RenderManager)
{
	if (m_UseLightVolumes)
		ExecuteDeferredShadingUsingLightVolumes(RenderManager);
	else ExecuteDeferredShading(RenderManager);
} 

void CDeferredShadingSceneRendererCommand::ExecuteDeferredShading(CRenderManager &RenderManager)
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
	for (size_t i = 0; i < l_Size; ++i)
	{
		/*Only 1 light*/
		if (l_Lights[i]->GetActive())
		{
			CEngine::GetSingleton().GetEffectManager()->SetLightConstants(0, l_Lights[i]);
			m_RenderableObjectTechnique->GetEffectTechnique()->SetConstantBuffer(1, &CEffectManager::m_LightEffectParameters);
			RenderManager.DrawScreenQuad(m_RenderableObjectTechnique->GetEffectTechnique(), NULL, 0, 0, 1.0f, 1.0f, CColor(v4fZERO));
		}
	}

	CEngine::GetSingleton().GetRenderManager()->GetContextManager()->DisableAlphaBlendState();
}
void CDeferredShadingSceneRendererCommand::ExecuteDeferredShadingUsingLightVolumes(CRenderManager &RenderManager)
{
	CEngine::GetSingleton().GetRenderManager()->GetContextManager()->SetAlphaBlendState(m_EnabledAlphaBlendState);

	CLightManager* l_LightManager = CEngine::GetSingleton().GetLightManager();
	std::vector<CLight*> l_Lights = l_LightManager->GetResourcesVector();
	size_t l_Size = l_Lights.size();

	int l_Width = RenderManager.GetContextManager()->GetFrameBufferWidth();
	int l_Height = RenderManager.GetContextManager()->GetFrameBufferHeight();

	ActivateTextures();

	for (size_t i = 0; i<l_Size; ++i)
	{
		/*Only 1 light*/
		if (l_Lights[i]->GetActive())
		{
			CEngine::GetSingleton().GetEffectManager()->SetLightConstants(0, l_Lights[i]);
			m_RenderableObjectTechnique->GetEffectTechnique()->SetConstantBuffer(1, &CEffectManager::m_LightEffectParameters);

			if (!l_Lights[i]->GetType() == CLight::OMNI) //
			{
				//RenderManager.GetContextManager()->SetRasterizerState(CContextManager::RS_SOLID);
				RenderManager.DrawScreenQuad(m_RenderableObjectTechnique->GetEffectTechnique(), NULL, 0, 0, 1.0f, 1.0f, CColor(v4fZERO));
			}
			else 
			{
				float l_LightRadius = l_Lights[i]->GetEndRangeAttenuation();
				Mat44f l_Scale;
				l_Scale.SetFromScale(l_LightRadius, l_LightRadius, l_LightRadius);

				Mat44f l_LightTransform = l_Lights[i]->GetTransform();
				
				RenderManager.GetContextManager()->SetWorldMatrix(l_Scale*l_LightTransform);
				if (IsCameraInsideLight(RenderManager,(COmniLight*)l_Lights[i]))
				{
					RenderManager.GetContextManager()->SetRasterizerState(CContextManager::RS_CULL_BACK);
					RenderManager.GetContextManager()->SetDepthStencilState(CContextManager::DSS_DEPTH_ON);
				}
				else
				{
					RenderManager.GetContextManager()->SetDepthStencilState(CContextManager::DSS_OFF);
					RenderManager.GetContextManager()->SetRasterizerState(CContextManager::RS_CULL_FRONT);
				}

				m_Sphere->Render(&RenderManager);
				
				/*float l_X = 0.0f;
				float l_Y = 0.0f;
				float l_Width = 0.0f;
				float l_Height = 0.0f;

				RenderManager.DrawScreenQuad(m_RenderableObjectTechnique->GetEffectTechnique(), NULL, l_X, l_Y, l_Width, l_Height, CColor(v4fZERO));*/
			}
		}
	}

	CEngine::GetSingleton().GetRenderManager()->GetContextManager()->DisableAlphaBlendState();
}

bool CDeferredShadingSceneRendererCommand::IsCameraInsideLight(CRenderManager &RenderManager, COmniLight* Light)
{
	Vect3f l_CameraPos = RenderManager.GetCurrentCamera().GetPosition();
	Vect3f l_LightPosition = Light->GetPosition();

	float l_Distance = (l_CameraPos - l_LightPosition).Length();

	if (l_Distance >= Light->GetEndRangeAttenuation()*1.2f)
	{
		//Use the front-face cull method as described above with those render states. Turn the stencil buffer off and don't mess with it.
		return false;
	}
	else
	{
		//(The first thing you need to (do is make sure that on your depth / stencil buffer, the stencil buffer is one solid value all the way across.I make my value 4.0 all over it.
		return true;
	}
}