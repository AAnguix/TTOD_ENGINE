#include "SetDepthStencilStateSceneRendererCommand.h"
#include "Render\RenderManager.h"
#include "Engine.h"
#include "Log\Log.h"
#include "XML\XMLTreeNode.h"

CSetDepthStencilStateSceneRendererCommand::CSetDepthStencilStateSceneRendererCommand(CXMLTreeNode &TreeNode) : CSceneRendererCommand(TreeNode)
{
	//m_DepthStencilState = nullptr;
	D3D11_DEPTH_STENCIL_DESC l_DepthStencilStateDescription;
	ZeroMemory(&l_DepthStencilStateDescription,sizeof(D3D11_DEPTH_STENCIL_DESC));
	l_DepthStencilStateDescription.DepthEnable=TreeNode.GetBoolProperty("enable_z_test", true) ? TRUE : FALSE;
	l_DepthStencilStateDescription.DepthWriteMask=TreeNode.GetBoolProperty("write_z_buffer", true) ? D3D11_DEPTH_WRITE_MASK_ALL :
	D3D11_DEPTH_WRITE_MASK_ZERO;
	l_DepthStencilStateDescription.DepthFunc=D3D11_COMPARISON_LESS_EQUAL;
	l_DepthStencilStateDescription.StencilEnable=TreeNode.GetBoolProperty("enable_stencil", false) ? TRUE : FALSE;
	
	if(FAILED(CEngine::GetSingleton().GetRenderManager()->GetContextManager()->GetDevice()->CreateDepthStencilState(&l_DepthStencilStateDescription, &m_DepthStencilState)))
	{
		CEngine::GetSingleton().GetLogManager()->Log("Error on creating CSetDepthStencilStateSceneRendererCommand");
	}

	//TODO ERROR
}
CSetDepthStencilStateSceneRendererCommand::~CSetDepthStencilStateSceneRendererCommand()
{
	m_DepthStencilState->Release();
	m_DepthStencilState = 0;
}

void CSetDepthStencilStateSceneRendererCommand::Execute(CRenderManager &RenderManager) 
{  
	RenderManager.GetContextManager()->GetDeviceContext()->OMSetDepthStencilState(m_DepthStencilState, 0);
}





