#include "EnableAdditiveAlphaBlendSceneRendererCommand.h"
#include "Render\RenderManager.h"
#include "Engine\Engine.h"
#include "Render\ContextManager.h"

CEnableAdditiveAlphaBlendSceneRendererCommand::CEnableAdditiveAlphaBlendSceneRendererCommand(CXMLTreeNode &TreeNode) : CSceneRendererCommand(TreeNode)
{	
}

void CEnableAdditiveAlphaBlendSceneRendererCommand::Execute(CRenderManager &RenderManager)
{
	RenderManager.GetContextManager()->EnableAdditiveAlphaBlendState();
}


