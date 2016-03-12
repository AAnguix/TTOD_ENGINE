#include "RenderDebugLightsSceneRendererCommand.h"
#include "Render\RenderManager.h"

CRenderDebugLightsSceneRendererCommand::CRenderDebugLightsSceneRendererCommand(CXMLTreeNode &TreeNode): CSceneRendererCommand(TreeNode) 
{

}

void CRenderDebugLightsSceneRendererCommand::Execute(CRenderManager &RenderManager)
{
	RenderManager.RenderDebugLights();
}
	
