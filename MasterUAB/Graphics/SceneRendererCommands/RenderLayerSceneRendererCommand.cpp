#include "RenderLayerSceneRendererCommand.h"
#include "XML\XMLTreeNode.h"
#include "Render\RenderManager.h"
#include "RenderableObjects\LayerManager.h"
#include "Engine\Engine.h"

CRenderLayerSceneRendererCommand::CRenderLayerSceneRendererCommand(CXMLTreeNode &TreeNode): CSceneRendererCommand(TreeNode),m_Layer(nullptr)
{
	std::string l_LayerName = TreeNode.GetPszProperty("layer");
	m_Layer = CEngine::GetSingleton().GetLayerManager()->GetResource(l_LayerName);
}

void CRenderLayerSceneRendererCommand::Execute(CRenderManager &RenderManager) 
{
	if(m_Layer!=nullptr)
		m_Layer->Render(&RenderManager);
}
	
