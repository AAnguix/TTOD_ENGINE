#include "RenderGUISceneRendererCommand.h"
#include "Engine\Engine.h"
#include "Render\ContextManager.h"
#include "GUIManager.h"

CRenderGUISceneRendererCommand::CRenderGUISceneRendererCommand(CXMLTreeNode &TreeNode): CSceneRendererCommand(TreeNode)
{
}  

void CRenderGUISceneRendererCommand::Execute(CRenderManager &RenderManager)
{
	CEngine::GetSingleton().GetGUIManager()->Render(&RenderManager);
}

	
