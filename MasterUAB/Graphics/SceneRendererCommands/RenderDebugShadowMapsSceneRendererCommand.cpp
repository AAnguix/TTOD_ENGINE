#include "RenderDebugShadowMapsSceneRendererCommand.h"
#include "Lights\LightManager.h"
#include "Engine\Engine.h"
#include "RenderableObjects\RenderableObjectTechniqueManager.h"
#include "Textures\DynamicTexture.h"
#include "Render\RenderManager.h"

CRenderDebugShadowMapsSceneRendererCommand::CRenderDebugShadowMapsSceneRendererCommand(CXMLTreeNode &TreeNode) : CSceneRendererCommand(TreeNode)
{

}


void CRenderDebugShadowMapsSceneRendererCommand::Execute(CRenderManager &RenderManager)
{
	CLightManager* l_LightManager = CEngine::GetSingleton().GetLightManager();
	std::vector<CLight*> l_Lights = l_LightManager->GetResourcesVector();
	size_t l_Size = l_Lights.size();

	CRenderableObjectTechnique* l_Technique = CEngine::GetSingleton().GetRenderableObjectTechniqueManager()->GetResource("MV_POSITION4_COLOR_TEXTURE_VERTEX");

	for(size_t i=0;i<l_Size;++i)
	{
		if(l_Lights[i]->GetGenerateShadowMap() && l_Lights[i]->GetShadowMap()!=NULL)
		{
			RenderManager.DrawScreenQuad(l_Technique->GetEffectTechnique(),(CTexture*)l_Lights[i]->GetBlackAndWhiteMap(),0, 0, 1, 1, CColor(0.0, 0.0, 0.0, 1.0));
		}
	}
}



