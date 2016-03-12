#ifndef _SETMATRICESSCENERENDERERCOMMAND_H
#define _SETMATRICESSCENERENDERERCOMMAND_H

#include "SceneRendererCommand.h"
class CRenderManager;
class CXMLTreeNode;

class CSetMatricesSceneRendererCommand : public CSceneRendererCommand 
{ 
	public:  
		CSetMatricesSceneRendererCommand(CXMLTreeNode &TreeNode);  
		void Execute(CRenderManager &RenderManager); 
};	
#endif
	




