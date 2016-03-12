#ifndef _RENDERDEBUGGUISCENERENDERERCOMMAND_H
#define _RENDERDEBUGGUISCENERENDERERCOMMAND_H

#include "SceneRendererCommand.h"

class CRenderDebugGUISceneRendererCommand : public CSceneRendererCommand 
{ 
	public:  
		CRenderDebugGUISceneRendererCommand(CXMLTreeNode &TreeNode);  
		virtual void Execute(CRenderManager &RenderManager); 
};

#endif
