#include "DeferredShadingSceneRendererCommand.h"
#include "Engine.h"
#include "RenderableObjects\RenderableObjectTechniqueManager.h"
#include "Render\RenderManager.h"
#include "Lights\LightManager.h"
#include "StaticMeshes\StaticMeshManager.h"
#include "Render\ContextManager.h"
#include "Lights\OmniLight.h"
#include "Utils\3DElement.h"
#include "XML\XMLTreeNode.h"

CDeferredShadingSceneRendererCommand::CDeferredShadingSceneRendererCommand(CXMLTreeNode &TreeNode)
:CStagedTexturedSceneRendererCommand(TreeNode)
,m_RenderableObjectTechnique(nullptr)
,m_UseLightVolumes(nullptr)
,m_DepthStencilState(nullptr)
,m_SphereFirstPass(nullptr)
{
	m_UseLightVolumes = TreeNode.GetBoolProperty("use_light_volumes", false);
	
	m_RenderableObjectTechnique = CEngine::GetSingleton().GetRenderableObjectTechniqueManager()->GetResource("MV_POSITION4_COLOR_TEXTURE_VERTEX");
	m_SphereFirstPass = CEngine::GetSingleton().GetStaticMeshManager()->GetResource("deferred_shading_sphere");

	//m_SphereSecondPass = CEngine::GetSingleton().GetStaticMeshManager()->GetResource("deferred_shading_second_sphere");
	/*if (m_Sphere == nullptr)
	{
	CEngine::GetSingleton().GetLogManager()->Log("Can't find sphere to optimize deferred shading");
	assert(false);
	}*/
}

CDeferredShadingSceneRendererCommand::~CDeferredShadingSceneRendererCommand()
{
	if (m_DepthStencilState)
	{
		m_DepthStencilState->Release();
		m_DepthStencilState = 0;
	}
}

void CDeferredShadingSceneRendererCommand::Execute(CRenderManager &RenderManager)
{
	if (m_UseLightVolumes)
		ExecuteDeferredShadingUsingLightVolumes(RenderManager);
	else ExecuteDeferredShading(RenderManager);
}

void CDeferredShadingSceneRendererCommand::ExecuteDeferredShading(CRenderManager &RenderManager)
{
	CEngine::GetSingleton().GetRenderManager()->GetContextManager()->EnableDeferredShadingBlendState();

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

			if (l_Lights[i]->GetType() != CLight::OMNI) //
			{
				RenderManager.GetContextManager()->SetRasterizerState(CContextManager::RS_SOLID);
				RenderManager.DrawScreenQuad(m_RenderableObjectTechnique->GetEffectTechnique(), NULL, 0, 0, 1.0f, 1.0f, CColor(v4fZERO));
			}
			else
			{
				float l_LightRadius = l_Lights[i]->GetEndRangeAttenuation();
				Mat44f l_Scale;
				l_Scale.SetFromScale(l_LightRadius, l_LightRadius, l_LightRadius);

				Mat44f l_LightTransform = l_Lights[i]->GetTransform();

				RenderManager.GetContextManager()->SetWorldMatrix(l_Scale*l_LightTransform);

				FirstPast(RenderManager);
				CRenderableObjectTechnique* l_Technique  = CEngine::GetSingleton().GetRenderableObjectTechniqueManager()->GetResource("deferred_shading_omnilight_sphere_renderable_object_technique");
				m_SphereFirstPass->Render(&RenderManager, l_Technique);
				SecondPass(RenderManager);
				m_SphereFirstPass->Render(&RenderManager);
			}
		}
	}

	CEngine::GetSingleton().GetRenderManager()->GetContextManager()->DisableAlphaBlendState();
}

void CDeferredShadingSceneRendererCommand::FirstPast(CRenderManager &RenderManager)
{
	RenderManager.GetContextManager()->EnableDeferredShadingBlendState();
	RenderManager.GetContextManager()->SetRasterizerState(CContextManager::RS_CULL_BACK); //Front (near) faces only

	D3D11_DEPTH_STENCIL_DESC l_DepthStencilStateDescription;
	ZeroMemory(&l_DepthStencilStateDescription, sizeof(D3D11_DEPTH_STENCIL_DESC));
	
	l_DepthStencilStateDescription.DepthEnable = true; /*Enable Z-Test*/
	l_DepthStencilStateDescription.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO; //Z-write is disabled
	l_DepthStencilStateDescription.DepthFunc = D3D11_COMPARISON_LESS_EQUAL; //Z function is 'Less/Equal'
	
	l_DepthStencilStateDescription.StencilEnable = true;
	l_DepthStencilStateDescription.StencilWriteMask = 0xff; //Stencil test result does not modify Stencil buffer

	l_DepthStencilStateDescription.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	l_DepthStencilStateDescription.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	l_DepthStencilStateDescription.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	l_DepthStencilStateDescription.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	l_DepthStencilStateDescription.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	l_DepthStencilStateDescription.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR_SAT; //Z-Fail writes non-zero value to Stencil buffer (for example, 'Increment-Saturate')
	l_DepthStencilStateDescription.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	l_DepthStencilStateDescription.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	HRESULT l_Result = CEngine::GetSingleton().GetRenderManager()->GetContextManager()->GetDevice()->CreateDepthStencilState(&l_DepthStencilStateDescription, &m_DepthStencilState);
	UINT l_StencilRef = 0;
	RenderManager.GetContextManager()->GetDeviceContext()->OMSetDepthStencilState(m_DepthStencilState, l_StencilRef);
}

void CDeferredShadingSceneRendererCommand::SecondPass(CRenderManager &RenderManager)
{
	RenderManager.GetContextManager()->EnableDeferredShadingBlendState();
	RenderManager.GetContextManager()->SetRasterizerState(CContextManager::RS_CULL_FRONT); //Back(far) faces only

	D3D11_DEPTH_STENCIL_DESC l_DepthStencilStateDescription;
	ZeroMemory(&l_DepthStencilStateDescription, sizeof(D3D11_DEPTH_STENCIL_DESC));
	l_DepthStencilStateDescription.DepthEnable = true;
	l_DepthStencilStateDescription.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO; //Z - write is disabled
	l_DepthStencilStateDescription.StencilReadMask = 0xff;
	l_DepthStencilStateDescription.StencilWriteMask = 0xff;
	l_DepthStencilStateDescription.DepthFunc = D3D11_COMPARISON_GREATER_EQUAL; //Z function is 'Greater/Equal'
	l_DepthStencilStateDescription.StencilEnable = true;

	l_DepthStencilStateDescription.BackFace.StencilFailOp = D3D11_STENCIL_OP_ZERO;
	l_DepthStencilStateDescription.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_ZERO;
	l_DepthStencilStateDescription.BackFace.StencilPassOp = D3D11_STENCIL_OP_ZERO;
	l_DepthStencilStateDescription.BackFace.StencilFunc = D3D11_COMPARISON_EQUAL;

	l_DepthStencilStateDescription.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	l_DepthStencilStateDescription.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	l_DepthStencilStateDescription.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	l_DepthStencilStateDescription.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	//Stencil function is 'Equal' (Stencil ref = zero)

	HRESULT l_Result = CEngine::GetSingleton().GetRenderManager()->GetContextManager()->GetDevice()->CreateDepthStencilState(&l_DepthStencilStateDescription, &m_DepthStencilState);

	UINT l_StencilRef = 0;
	RenderManager.GetContextManager()->GetDeviceContext()->OMSetDepthStencilState(m_DepthStencilState, l_StencilRef);
	//Always clears Stencil to zero
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