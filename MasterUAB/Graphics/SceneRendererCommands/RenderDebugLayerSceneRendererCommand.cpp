#include "RenderDebugLayerSceneRendererCommand.h"
#include "XML\XMLTreeNode.h"
#include "Render\RenderManager.h"
#include "RenderableObjects\LayerManager.h"
#include "Engine\Engine.h"

CRenderDebugLayerSceneRendererCommand::CRenderDebugLayerSceneRendererCommand(CXMLTreeNode &TreeNode): CSceneRendererCommand(TreeNode) 
{
	std::string l_LayerName = TreeNode.GetPszProperty("layer");
	m_Layer = CEngine::GetSingleton().GetLayerManager()->GetResource(l_LayerName);
} 

void CRenderDebugLayerSceneRendererCommand::Execute(CRenderManager &RenderManager)
{
	RenderManager.RenderDebugLayer(m_Layer);
} 
