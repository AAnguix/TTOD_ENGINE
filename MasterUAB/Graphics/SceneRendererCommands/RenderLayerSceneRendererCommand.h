#ifndef _RENDERLAYERSCENERENDERERCOMMAND_H
#define _RENDERLAYERSCENERENDERERCOMMAND_H

#include "SceneRendererCommand.h"
class CRenderableObjectsManager;
class CRenderManager;
class CXMLTreeNode;

class CRenderLayerSceneRendererCommand : public CSceneRendererCommand 
{ 
	private:  
		CRenderableObjectsManager  *m_Layer; 

	public:  
		CRenderLayerSceneRendererCommand(CXMLTreeNode &TreeNode);  
		void Execute(CRenderManager &RenderManager); 
};

#endif
	
