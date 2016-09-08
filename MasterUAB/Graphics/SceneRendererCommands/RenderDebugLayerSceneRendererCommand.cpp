#include "RenderDebugLayerSceneRendererCommand.h"
#include "XML\XMLTreeNode.h"
#include "Render\RenderManager.h"
#include "RenderableObjects\LayerManager.h"
#include "Engine\Engine.h"

CRenderDebugLayerSceneRendererCommand::CRenderDebugLayerSceneRendererCommand(CXMLTreeNode &TreeNode): CSceneRendererCommand(TreeNode) 
,m_Layer(CEngine::GetSingleton().GetLayerManager()->GetResource(TreeNode.GetPszProperty("layer")))
{
} 

void CRenderDebugLayerSceneRendererCommand::Execute(CRenderManager &RenderManager)
{
	RenderManager.RenderDebugLayer(m_Layer);
} 
