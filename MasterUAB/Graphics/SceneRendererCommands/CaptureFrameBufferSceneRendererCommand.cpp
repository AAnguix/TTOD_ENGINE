#include "CaptureFrameBufferSceneRendererCommand.h"
#include "CaptureFrameBufferTexture.h"

CCaptureFrameBufferSceneRendererCommand::CCaptureFrameBufferSceneRendererCommand(CXMLTreeNode &TreeNode) : CStagedTexturedSceneRendererCommand(TreeNode)
{
	
}

void CCaptureFrameBufferSceneRendererCommand::Execute(CRenderManager &RenderManager)
{
	size_t l_ID = m_StageTextures[0].m_StageId;
	((CCaptureFrameBufferTexture*)m_StageTextures[0].m_Texture)->Capture(l_ID);
}



