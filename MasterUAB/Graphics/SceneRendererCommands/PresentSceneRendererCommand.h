#ifndef _PRESENTSCENERENDERERCOMMAND_H
#define _PRESENTSCENERENDERERCOMMAND_H

#include "SceneRendererCommand.h"
class CRenderManager;
class CXMLTreeNode;

class CPresentSceneRendererCommand : public CSceneRendererCommand 
{ 
	public:  
		CPresentSceneRendererCommand(CXMLTreeNode &TreeNode);  
		void Execute(CRenderManager &RenderManager); 
}; 

#endif
	




