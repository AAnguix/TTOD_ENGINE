#include "DrawQuadRendererCommand.h"
#include "RenderableObjects\RenderableObjectTechniqueManager.h"
#include "Render\RenderManager.h"
#include "Materials\Material.h"
#include "Engine.h"

CDrawQuadRendererCommand::CDrawQuadRendererCommand(CXMLTreeNode &TreeNode): CStagedTexturedSceneRendererCommand(TreeNode),m_RenderableObjectTechnique(0)
{
	if(m_Material)
	{
		m_RenderableObjectTechnique=m_Material->GetRenderableObjectTechnique();
	}
	else
	{
		m_RenderableObjectTechnique=CEngine::GetSingleton().GetRenderableObjectTechniqueManager()->GetResource("MV_POSITION4_COLOR_TEXTURE_VERTEX");
	}
}  

void CDrawQuadRendererCommand::Execute(CRenderManager &RenderManager)
{	
	if(m_Material)
		m_Material->Apply();
	
	ActivateTextures();
	
	//int l_Width = RenderManager.GetContextManager()->GetFrameBufferWidth();
	//int l_Height = RenderManager.GetContextManager()->GetFrameBufferHeight();

	RenderManager.DrawScreenQuad(m_RenderableObjectTechnique->GetEffectTechnique(),NULL,0,0,1.0f,1.0f,CColor(1.0f, 1.0f, 1.0f, 1.0f));
} 
