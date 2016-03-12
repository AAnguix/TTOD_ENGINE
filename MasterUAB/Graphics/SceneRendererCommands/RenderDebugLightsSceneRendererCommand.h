#ifndef _RENDERDEBUGLIGHTSSCENERENDERERCOMMAND_H
#define _RENDERDEBUGLIGHTSSCENERENDERERCOMMAND_H

#include "SceneRendererCommand.h"

class CRenderDebugLightsSceneRendererCommand : public CSceneRendererCommand 
{ 
	public:  
		CRenderDebugLightsSceneRendererCommand(CXMLTreeNode &TreeNode);  
		virtual void Execute(CRenderManager &RenderManager); 
};

#endif
	
