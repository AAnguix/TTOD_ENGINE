#include "SetLightConstantsSceneRendererCommand.h"
#include "Engine.h"
#include "Effects\EffectManager.h"

CSetLightConstantsSceneRendererCommand::CSetLightConstantsSceneRendererCommand(CXMLTreeNode &TreeNode): CSceneRendererCommand(TreeNode) 
{
	
}

void CSetLightConstantsSceneRendererCommand::Execute(CRenderManager &RenderManager) 
{
	CEngine::GetSingleton().GetEffectManager()->SetLightsConstants(4);
}
	
