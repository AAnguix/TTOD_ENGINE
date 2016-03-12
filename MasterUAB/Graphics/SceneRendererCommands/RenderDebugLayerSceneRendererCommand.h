#ifndef _RENDERDEBUGLAYERSCENERENDERERCOMMAND_H
#define _RENDERDEBUGLAYERSCENERENDERERCOMMAND_H

#include "SceneRendererCommand.h"
class CRenderableObjectsManager;
class CRenderManager;
class CXMLTreeNode;

class CRenderDebugLayerSceneRendererCommand : public CSceneRendererCommand 
{ 
	private:  
		CRenderableObjectsManager    *m_Layer; 

	public:  
		CRenderDebugLayerSceneRendererCommand(CXMLTreeNode &TreeNode);  
		void Execute(CRenderManager &RenderManager); 
	}; 

#endif
	
