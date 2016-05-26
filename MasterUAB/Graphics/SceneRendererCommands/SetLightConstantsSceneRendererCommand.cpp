#include "SetLightConstantsSceneRendererCommand.h"
#include "Engine.h"
#include "Effects\EffectManager.h"

CSetLightConstantsSceneRendererCommand::CSetLightConstantsSceneRendererCommand(CXMLTreeNode &TreeNode): CSceneRendererCommand(TreeNode) 
{
	m_MaxLights = TreeNode.GetIntProperty("max_lights",4);
}

void CSetLightConstantsSceneRendererCommand::Execute(CRenderManager &RenderManager) 
{
	CEngine::GetSingleton().GetEffectManager()->SetLightsConstants(m_MaxLights);
}
	
