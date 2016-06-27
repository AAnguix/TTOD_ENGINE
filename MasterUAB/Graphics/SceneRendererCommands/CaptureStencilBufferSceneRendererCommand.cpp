#include "CaptureStencilBufferSceneRendererCommand.h"
#include "CaptureStencilBufferTexture.h"

CCaptureStencilBufferSceneRendererCommand::CCaptureStencilBufferSceneRendererCommand(CXMLTreeNode &TreeNode) 
:CStagedTexturedSceneRendererCommand(TreeNode)
{
	
}

void CCaptureStencilBufferSceneRendererCommand::Execute(CRenderManager &RenderManager)
{
	size_t l_ID = m_StageTextures[0].m_StageId;
	((CCaptureStencilBufferTexture*)m_StageTextures[0].m_Texture)->Capture(l_ID);
}



