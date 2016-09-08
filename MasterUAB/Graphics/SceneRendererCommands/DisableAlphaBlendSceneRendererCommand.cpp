#include "DisableAlphaBlendSceneRendererCommand.h"
#include "Render\RenderManager.h"
#include "Render\ContextManager.h"

CDisableAlphaBlendSceneRendererCommand::CDisableAlphaBlendSceneRendererCommand(CXMLTreeNode &TreeNode) : CSceneRendererCommand(TreeNode)
{
}

void CDisableAlphaBlendSceneRendererCommand::Execute(CRenderManager &RenderManager)
{
	RenderManager.GetContextManager()->DisableAlphaBlendState();
}


