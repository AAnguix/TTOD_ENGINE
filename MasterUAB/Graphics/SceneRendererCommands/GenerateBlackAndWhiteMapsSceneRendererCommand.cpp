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

	//We are going to render from camera perspective
	//RenderManager.GetContextManager()->SetMatrices(RenderManager.GetCurrentCamera());

	for (size_t i = 0; i<l_Size; ++i)
	{
		if (l_Lights[i]->GetActive() && l_Lights[i]->GetGenerateShadowMap())
		{
			l_Lights[i]->SetBlackAndWhiteMap(RenderManager); //Set BlackAndWhiteTexture as rendertarget
			//Doens't change matrix, beacuse we must render from camera perspective

			CEngine::GetSingleton().GetEffectManager()->SetBlackAndWhiteLightConstants(l_Lights[i]);
			//RenderManager.GetContextManager()->Clear(false,true);

			std::vector<CRenderableObjectsManager*> m_ROManager = l_Lights[i]->GetLayers();

#ifdef _DEBUG
			if (m_ROManager.size() == 0)
			{
				LOG("Error. Can't generate shadows because there must be at least one layer");
			}
#endif

			/*for(size_t j=0;j<m_ROManager.size();j++)
			{
			CLayerManager* l_Layer = CEngine::GetSingleton().GetLayerManager();
			l_Layer->Render(RenderManager, m_ROManager[j]->GetName());
			}*/
#pragma warning("Arturo hay que hacer bien el renderable object que no lo coja cada frame")
			CEffectTechnique *l_EffectTechnique = CEngine::GetSingleton().GetEffectManager()->GetResource("generate_black_and_white_map_technique");
			RenderManager.DrawScreenQuad(l_EffectTechnique, NULL, 0, 0, 1.0f, 1.0f, CColor(1.0f, 1.0f, 1.0f, 1.0f));
		}
	}

	RenderManager.GetContextManager()->UnsetRenderTargets();
	RenderManager.GetContextManager()->SetDefaultViewPort();
}



