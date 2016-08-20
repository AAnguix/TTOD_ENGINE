#include "GenerateBlackAndWhiteMapsSceneRendererCommand.h"
#include "Engine\Engine.h"
#include "Lights\LightManager.h"
#include "RenderableObjects\LayerManager.h"
#include "RenderableObjects\RenderableObjectTechniqueManager.h"
#include "Render\RenderManager.h"
#include "Log\Log.h"

CGenerateBlackAndWhiteMapsSceneRendererCommand::CGenerateBlackAndWhiteMapsSceneRendererCommand(CXMLTreeNode &TreeNode) : CStagedTexturedSceneRendererCommand(TreeNode)
{
	m_RenderableObjectTechnique = CEngine::GetSingleton().GetRenderableObjectTechniqueManager()->GetResource("generate_black_and_white_map_renderable_object_technique");
}

CGenerateBlackAndWhiteMapsSceneRendererCommand::~CGenerateBlackAndWhiteMapsSceneRendererCommand(){

}

bool x = true;
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
			l_Lights[i]->SetBlackAndWhiteMap(RenderManager, false); //Set BlackAndWhiteTexture as rendertarget
			//Doens't change matrix, beacuse we must render from camera perspective

			CEngine::GetSingleton().GetEffectManager()->SetBlackAndWhiteLightConstants(l_Lights[i]);
			RenderManager.DrawScreenQuad(m_RenderableObjectTechnique->GetEffectTechnique(), NULL, 0, 0, 1.0f, 1.0f, CColor(1.0f, 1.0f, 1.0f, 1.0f));

			//MIO
			if (x)
			{
				CMaterial* l_GaussianFilterMaterial = CEngine::GetSingleton().GetMaterialManager()->GetResource("GaussianFilterMaterial");
				CMaterial* l_GuiMaterial = CEngine::GetSingleton().GetMaterialManager()->GetResource("GUIMaterial");

				RenderManager.GetContextManager()->UnsetRenderTargets();
				RenderManager.GetContextManager()->SetDefaultViewPort();

				l_Lights[i]->SetBlackAndWhiteMap(RenderManager, true);
				CPoolRenderableObjectTechnique* l_GaussianFilterPool = CEngine::GetSingleton().GetRenderableObjectTechniqueManager()->GetPoolRenderableObjectTechniques().GetResource("gaussian_filter_pool_renderable_object_technique");
				l_GaussianFilterPool->Apply();
				l_Lights[i]->GetBlackAndWhiteMap()->Activate(0);
				l_GaussianFilterMaterial->Apply();

				RenderManager.DrawScreenQuad(l_GaussianFilterMaterial->GetRenderableObjectTechnique()->GetEffectTechnique(), NULL, 0, 0, 1.0f, 1.0f, CColor(0.0, 0.0, 0.0, 0.0));
			}
		}
	}

	RenderManager.GetContextManager()->UnsetRenderTargets();
	RenderManager.GetContextManager()->SetDefaultViewPort();
}



