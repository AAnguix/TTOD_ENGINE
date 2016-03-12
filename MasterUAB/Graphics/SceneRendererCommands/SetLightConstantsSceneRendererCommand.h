#ifndef _SETLIGHTCONSTANTSCENERENDERERCOMMAND_H
#define _SETLIGHTCONSTANTSCENERENDERERCOMMAND_H

#include "SceneRendererCommand.h"
class CRenderManager;
class CXMLTreeNode;

class CSetLightConstantsSceneRendererCommand : public CSceneRendererCommand 
{ 
	public:  
		CSetLightConstantsSceneRendererCommand(CXMLTreeNode &TreeNode);  
		void Execute(CRenderManager &RenderManager); 
};	

#endif
	
