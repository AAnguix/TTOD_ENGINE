#include "RenderDebugGridSceneRendererCommand.h"
#include "Render\RenderManager.h"

CRenderDebugGridSceneRendererCommand::CRenderDebugGridSceneRendererCommand(CXMLTreeNode &TreeNode) : CSceneRendererCommand(TreeNode)
{
}


void CRenderDebugGridSceneRendererCommand::Execute(CRenderManager &RenderManager)
{
	RenderManager.RenderDebugGrid();
}



