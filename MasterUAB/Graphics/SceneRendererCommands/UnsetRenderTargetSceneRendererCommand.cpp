#include"UnsetRenderTargetSceneRendererCommand.h"
#include "Render\RenderManager.h"

CUnsetRenderTargetSceneRendererCommand::CUnsetRenderTargetSceneRendererCommand(CXMLTreeNode &TreeNode):CSceneRendererCommand(TreeNode) 
{

}  

void CUnsetRenderTargetSceneRendererCommand::Execute(CRenderManager &RenderManager)
{
	RenderManager.GetContextManager()->UnsetRenderTargets();
}
