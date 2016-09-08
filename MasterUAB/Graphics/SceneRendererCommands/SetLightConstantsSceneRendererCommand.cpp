#include "SetLightConstantsSceneRendererCommand.h"
#include "Engine\Engine.h"
#include "Effects\EffectManager.h"
#include "XML\XMLTreeNode.h"

CSetLightConstantsSceneRendererCommand::CSetLightConstantsSceneRendererCommand(CXMLTreeNode &TreeNode): CSceneRendererCommand(TreeNode) 
,m_MaxLights(TreeNode.GetIntProperty("max_lights", 4))
{
	assert(m_MaxLights < 5);
}

void CSetLightConstantsSceneRendererCommand::Execute(CRenderManager &RenderManager) 
{
	CEngine::GetSingleton().GetEffectManager()->SetLightsConstants(m_MaxLights);
}
	
