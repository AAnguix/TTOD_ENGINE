#include "SetRenderTargetSceneRendererCommand.h"
#include "Textures\DynamicTexture.h"
#include "Render\RenderManager.h"

CSetRenderTargetSceneRendererCommand::CSetRenderTargetSceneRendererCommand(CXMLTreeNode &TreeNode): CStagedTexturedSceneRendererCommand(TreeNode)
{

} 

void CSetRenderTargetSceneRendererCommand::Execute(CRenderManager &RenderManager)
{
	ID3D11DepthStencilView *l_DepthStencilView=m_DynamicTextures.empty() ? NULL : m_DynamicTextures[0]->GetDynamicTexture()->GetDepthStencilView();
	
	/*TODO ERROR
	*/
	//if(!l_DepthStencilView)
	//	l_DepthStencilView=RenderManager.GetContextManager()->GetDepthStencilView();

	RenderManager.GetContextManager()->SetRenderTargets((UINT)m_DynamicTextures.size(), &m_RenderTargetViews[0], l_DepthStencilView==NULL ? RenderManager.GetContextManager()->GetDepthStencilView() : l_DepthStencilView);
}
