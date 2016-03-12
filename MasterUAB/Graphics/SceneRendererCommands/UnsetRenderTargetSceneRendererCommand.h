#ifndef _UNSETRENDERTARGETSCENERENDERERCOMMAND_H
#define _UNSETRENDERTARGETSCENERENDERERCOMMAND_H
#include "SceneRendererCommand.h"

class CUnsetRenderTargetSceneRendererCommand : public CSceneRendererCommand 
{ 
	public:  
		CUnsetRenderTargetSceneRendererCommand(CXMLTreeNode &TreeNode);  
		void Execute(CRenderManager &RenderManager);
	
};

#endif
	

