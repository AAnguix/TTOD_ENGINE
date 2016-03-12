#include "ClearSceneRendererCommand.h"
#include "XML\XMLTreeNode.h"
#include "Render\RenderManager.h"

CClearSceneRendererCommand::CClearSceneRendererCommand(CXMLTreeNode &TreeNode) : CSceneRendererCommand(TreeNode)
{
	m_RenderTarget = TreeNode.GetBoolProperty("render_target",false);
	m_DepthStencil = TreeNode.GetBoolProperty("depth_stencil",false);
}

void CClearSceneRendererCommand::Execute(CRenderManager &RenderManager)
{
	RenderManager.GetContextManager()->Clear(m_RenderTarget,m_DepthStencil);
}