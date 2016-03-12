#ifndef _SETRENDERTARGETSCENERENDERERCOMMAND_H
#define _SETRENDERTARGETSCENERENDERERCOMMAND_H

#include "StagedTexturedSceneRendererCommand.h"

class CSetRenderTargetSceneRendererCommand : public CStagedTexturedSceneRendererCommand 
{ 
	public:  
	CSetRenderTargetSceneRendererCommand(CXMLTreeNode &TreeNode);  
	void Execute(CRenderManager &RenderManager); 

};

#endif
	
