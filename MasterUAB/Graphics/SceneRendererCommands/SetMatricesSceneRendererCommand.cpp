#include "SetMatricesSceneRendererCommand.h"
#include "XML\XMLTreeNode.h"
#include "Render\RenderManager.h"

CSetMatricesSceneRendererCommand::CSetMatricesSceneRendererCommand(CXMLTreeNode &TreeNode): CSceneRendererCommand(TreeNode)  
{

}

void CSetMatricesSceneRendererCommand::Execute(CRenderManager &RenderManager)
{
	RenderManager.GetContextManager()->SetMatrices(RenderManager.GetCurrentCamera());
}
	





