#include "RenderDebugGUISceneRendererCommand.h"
#include "DebugHelper\DebugHelper.h"

CRenderDebugGUISceneRendererCommand::CRenderDebugGUISceneRendererCommand(CXMLTreeNode &TreeNode): CSceneRendererCommand(TreeNode)
{

}  

void CRenderDebugGUISceneRendererCommand::Execute(CRenderManager &RenderManager)
{
	CDebugHelper::GetDebugHelper()->Render();
}

	
