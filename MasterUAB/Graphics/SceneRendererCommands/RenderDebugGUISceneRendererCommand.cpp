#include "RenderDebugGUISceneRendererCommand.h"
#include "DebugHelper.h"

CRenderDebugGUISceneRendererCommand::CRenderDebugGUISceneRendererCommand(CXMLTreeNode &TreeNode): CSceneRendererCommand(TreeNode)
{

}  

void CRenderDebugGUISceneRendererCommand::Execute(CRenderManager &RenderManager)
{
	CDebugHelper::GetDebugHelper()->Render();
}

	
