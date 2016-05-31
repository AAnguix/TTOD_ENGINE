#include "EnableAdditiveAlphaBlendSceneRendererCommand.h"
#include "Render\RenderManager.h"
#include "Engine.h"

CEnableAdditiveAlphaBlendSceneRendererCommand::CEnableAdditiveAlphaBlendSceneRendererCommand(CXMLTreeNode &TreeNode) : CSceneRendererCommand(TreeNode)
{
	
}

void CEnableAdditiveAlphaBlendSceneRendererCommand::Execute(CRenderManager &RenderManager)
{
	RenderManager.GetContextManager()->EnableAdditiveAlphaBlendState();
}


