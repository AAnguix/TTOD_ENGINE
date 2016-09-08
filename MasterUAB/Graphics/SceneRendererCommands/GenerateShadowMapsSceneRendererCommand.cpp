#include "GenerateShadowMapsSceneRendererCommand.h"
#include "Engine\Engine.h"
#include "Lights\LightManager.h"
#include "RenderableObjects\LayerManager.h"
#include "RenderableObjects\RenderableObjectTechniqueManager.h"
#include "Render\RenderManager.h"
#include "Log\Log.h"
#include "Render\ContextManager.h"

CGenerateShadowMapsSceneRendererCommand::CGenerateShadowMapsSceneRendererCommand(CXMLTreeNode &TreeNode) : CSceneRendererCommand(TreeNode)
{
}

CGenerateShadowMapsSceneRendererCommand::~CGenerateShadowMapsSceneRendererCommand()
{
}

void CGenerateShadowMapsSceneRendererCommand::Execute(CRenderManager &RenderManager)
{
	CLightManager* l_LightManager = CEngine::GetSingleton().GetLightManager();
	std::vector<CLight*> l_Lights = l_LightManager->GetResourcesVector();
	size_t l_Size = l_Lights.size();

	for(size_t i=0;i<l_Size;++i)
	{
		if(l_Lights[i]->GetActive() && l_Lights[i]->GetGenerateShadowMap())
		{
			l_Lights[i]->SetShadowMap(RenderManager); //Set view,proj matrix

			RenderManager.GetContextManager()->Clear(false,true);
			
			std::vector<CRenderableObjectsManager*> m_ROManager = l_Lights[i]->GetLayers();

			#ifdef _DEBUG
			if (m_ROManager.size() == 0)
			{
				LOG("Error. Can't generate shadows because there must be at least one layer");
			}
			#endif

			for(size_t j=0;j<m_ROManager.size();j++)
			{
				CLayerManager* l_Layer = CEngine::GetSingleton().GetLayerManager();
				l_Layer->Render(RenderManager, m_ROManager[j]->GetName());
				//RenderManager.DrawScreenQuad(CEngine::GetSingleton().GetRenderableObjectTechniqueManager()->GetResource("MV_POSITION4_COLOR_TEXTURE_VERTEX")->GetEffectTechnique(),(CTexture*)l_Lights[i]->GetShadowMap(),0,0,1.0,1.0,CColor(1.0,0.0,0.0,1.0));
			}
		}
	}

	RenderManager.GetContextManager()->UnsetRenderTargets();
	//SetDefaultViewport poner en contextManager
	RenderManager.GetContextManager()->SetDefaultViewPort();

	/*D3D11_VIEWPORT l_Viewport;
	l_Viewport.Width=RenderManager.GetContextManager()->GetViewPort().Width;
	l_Viewport.Height=RenderManager.GetContextManager()->GetViewPort().Height;
	l_Viewport.MinDepth = 0.0f;
	l_Viewport.MaxDepth = 1.0f;
	l_Viewport.TopLeftX = 0.0f;
	l_Viewport.TopLeftY = 0.0f;
	RenderManager.GetContextManager()->GetDeviceContext()->RSSetViewports(1, &l_Viewport);*/

}



