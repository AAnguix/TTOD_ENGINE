#include "SetPoolRenderableObjectsTechniqueSceneRendererCommand.h"
#include "RenderableObjects\RenderableObjectTechniqueManager.h"
#include "Engine\Engine.h"
#include "XML\XMLTreeNode.h"

CSetPoolRenderableObjectsTechniqueSceneRendererCommand::CSetPoolRenderableObjectsTechniqueSceneRendererCommand(CXMLTreeNode &TreeNode): CSceneRendererCommand(TreeNode) 
{
	std::string l_PoolName = TreeNode.GetPszProperty("pool");
	m_Pool = CEngine::GetSingleton().GetRenderableObjectTechniqueManager()->GetPoolRenderableObjectTechniques().GetResource(l_PoolName);
}

void CSetPoolRenderableObjectsTechniqueSceneRendererCommand::Execute(CRenderManager &RenderManager)
{
	m_Pool->Apply();
}
