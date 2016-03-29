#include "RenderGUISceneRendererCommand.h"
#include "Engine.h"
#include "GUIManager.h"

CRenderGUISceneRendererCommand::CRenderGUISceneRendererCommand(CXMLTreeNode &TreeNode): CSceneRendererCommand(TreeNode)
{

}  

void CRenderGUISceneRendererCommand::Execute(CRenderManager &RenderManager)
{
	CEngine::GetSingleton().GetGUIManager()->Render(&RenderManager);
}

	
