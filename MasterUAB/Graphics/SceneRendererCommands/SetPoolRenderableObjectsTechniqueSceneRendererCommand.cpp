#include "SetPoolRenderableObjectsTechniqueSceneRendererCommand.h"
#include "RenderableObjects\RenderableObjectTechniqueManager.h"
#include "Engine\Engine.h"
#include "XML\XMLTreeNode.h"

CSetPoolRenderableObjectsTechniqueSceneRendererCommand::CSetPoolRenderableObjectsTechniqueSceneRendererCommand(CXMLTreeNode &TreeNode): CSceneRendererCommand(TreeNode) 
,m_Pool(CEngine::GetSingleton().GetRenderableObjectTechniqueManager()->GetPoolRenderableObjectTechniques().GetResource(TreeNode.GetPszProperty("pool")))
{
}

void CSetPoolRenderableObjectsTechniqueSceneRendererCommand::Execute(CRenderManager &RenderManager)
{
	m_Pool->Apply();
}
