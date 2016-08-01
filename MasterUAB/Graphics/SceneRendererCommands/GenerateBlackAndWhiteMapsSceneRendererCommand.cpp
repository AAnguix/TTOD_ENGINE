#include "GenerateBlackAndWhiteMapsSceneRendererCommand.h"
#include "Engine\Engine.h"
#include "Lights\LightManager.h"
#include "RenderableObjects\LayerManager.h"
#include "RenderableObjects\RenderableObjectTechniqueManager.h"
#include "Render\RenderManager.h"
#include "Log\Log.h"

CGenerateBlackAndWhiteMapsSceneRendererCommand::CGenerateBlackAndWhiteMapsSceneRendererCommand(CXMLTreeNode &TreeNode) : CStagedTexturedSceneRendererCommand(TreeNode)
{

}

CGenerateBlackAndWhiteMapsSceneRendererCommand::~CGenerateBlackAndWhiteMapsSceneRendererCommand(){

}

void CGenerateBlackAndWhiteMapsSceneRendererCommand::Execute(CRenderManager &RenderManager)
{
	CLightManager* l_LightManager = CEngine::GetSingleton().GetLightManager();
	std::vector<CLight*> l_Lights = l_LightManager->GetResourcesVector();
	size_t l_Size = l_Lights.size();

	ActivateTextures();

	for(size_t i=0;i<l_Size;++i)
	{
		if(l_Lights[i]->GetActive() && l_Lights[i]->GetGenerateShadowMap())
		{
			l_Lights[i]->SetBlackAndWhiteMap(RenderManager); //Set view,proj matrix

			CEngine::GetSingleton().GetEffectManager()->SetLightConstants(0, l_Lights[i]);

			RenderManager.GetContextManager()->Clear(false,true);
			
			std::vector<CRenderableObjectsManager*> m_ROManager = l_Lights[i]->GetLayers();

			#ifdef _DEBUG
			if (m_ROManager.size() == 0)
			{
				LOG("Error. Can't generate shadows because there must be at least one layer");
			}
			#endif

			for(size_t i=0;i<m_ROManager.size();i++)
			{
				CLayerManager* l_Layer = CEngine::GetSingleton().GetLayerManager();
				l_Layer->Render(RenderManager, m_ROManager[i]->GetName());
			}
		}
	}

	RenderManager.GetContextManager()->UnsetRenderTargets();
	RenderManager.GetContextManager()->SetDefaultViewPort();
}



