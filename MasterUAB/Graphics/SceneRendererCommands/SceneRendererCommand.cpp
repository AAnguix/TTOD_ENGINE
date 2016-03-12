#include "SceneRendererCommand.h"
#include "Engine.h"
#include "Materials\MaterialManager.h"

CSceneRendererCommand::CSceneRendererCommand(CXMLTreeNode &TreeNode) : CNamed(TreeNode.GetName()),CActive(TreeNode),m_Material(0)
{
	std::string l_MaterialName = TreeNode.GetPszProperty("material","");
	m_Material = CEngine::GetSingleton().GetMaterialManager()->GetResource(l_MaterialName);
}

CSceneRendererCommand::~CSceneRendererCommand()
{

} 

void CSceneRendererCommandExecute(CRenderManager &RenderManager)
{

}
	




