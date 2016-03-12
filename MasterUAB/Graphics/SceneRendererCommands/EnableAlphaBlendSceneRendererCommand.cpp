#include "EnableAlphaBlendSceneRendererCommand.h"
#include "Render\RenderManager.h"

CEnableAlphaBlendSceneRendererCommand::CEnableAlphaBlendSceneRendererCommand(CXMLTreeNode &TreeNode) : CSceneRendererCommand(TreeNode)
{

}


void CEnableAlphaBlendSceneRendererCommand::Execute(CRenderManager &RenderManager)
{
	RenderManager.GetContextManager()->EnableAlphaBlendState();
}


