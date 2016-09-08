#include "EnableAlphaBlendSceneRendererCommand.h"
#include "Render\RenderManager.h"
#include "Render\ContextManager.h"

CEnableAlphaBlendSceneRendererCommand::CEnableAlphaBlendSceneRendererCommand(CXMLTreeNode &TreeNode) : CSceneRendererCommand(TreeNode)
{

}


void CEnableAlphaBlendSceneRendererCommand::Execute(CRenderManager &RenderManager)
{
	RenderManager.GetContextManager()->EnableAlphaBlendState();
}


