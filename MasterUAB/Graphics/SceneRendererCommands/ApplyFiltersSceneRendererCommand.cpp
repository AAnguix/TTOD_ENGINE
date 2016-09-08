#include "SceneRendererCommands\ApplyFiltersSceneRendererCommand.h"
#include "Render\RenderManager.h"
#include "Engine\Engine.h"
#include "Materials\MaterialManager.h"
#include "RenderableObjects\RenderableObjectTechniqueManager.h"
#include "Render\ContextManager.h"

CApplyFiltersSceneRendererCommand::CApplyFiltersSceneRendererCommand(CXMLTreeNode &TreeNode): CStagedTexturedSceneRendererCommand(TreeNode)
{
}

void CApplyFiltersSceneRendererCommand::Execute(CRenderManager &RenderManager)
{
	if(m_StageTextures.size()>0)
	{
		if(m_StageTextures[0].m_Texture!=NULL)
		{
			for(size_t i=0;i<m_DynamicTextures.size();++i)
			{
				RenderManager.GetContextManager()->SetRenderTargets(1, &m_RenderTargetViews[i], RenderManager.GetContextManager()->GetDepthStencilView());
				CMaterial* l_Material=nullptr;

				if (i == 0)	{ m_StageTextures[0].Activate(); }
				else
				{
					l_Material = m_DynamicTextures[i-1]->GetMaterial();
					m_DynamicTextures[i-1]->GetDynamicTexture()->Activate(0);
				}	
				
				if(l_Material!=NULL)
				{	
					l_Material->Apply(); 
					RenderManager.DrawScreenQuad(l_Material->GetRenderableObjectTechnique()->GetEffectTechnique(), NULL,0,0,1.0f,1.0f,CColor(0.0,0.0,0.0,1.0));
				}
				else
				{
					RenderManager.DrawScreenQuad(CEngine::GetSingleton().GetRenderableObjectTechniqueManager()->GetResource("MV_POSITION4_COLOR_TEXTURE_VERTEX")->GetEffectTechnique(), NULL,0,0,1.0f,1.0f,CColor(0.0,0.0,0.0,1.0));
				}
				 
				RenderManager.GetContextManager()->UnsetRenderTargets();
			}
		}
	}
}
