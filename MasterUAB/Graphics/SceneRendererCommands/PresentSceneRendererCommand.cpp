#include "PresentSceneRendererCommand.h"
#include "XML\XMLTreeNode.h"
#include "Render\RenderManager.h"
#include "Render\ContextManager.h"

CPresentSceneRendererCommand:: CPresentSceneRendererCommand(CXMLTreeNode &TreeNode): CSceneRendererCommand(TreeNode)
{
} 

void CPresentSceneRendererCommand:: Execute(CRenderManager &RenderManager)
{
	RenderManager.GetContextManager()->Present();
}
	




