#ifndef _CLEARSCENERENDERERCOMMAND_H
#define _CLEARSCENERENDERERCOMMAND_H

#include "SceneRendererCommand.h"
class CRenderManager;
class CXMLTreeNode;

class CClearSceneRendererCommand : public CSceneRendererCommand 
{ 
	protected:  
		bool m_RenderTarget;  
		bool m_DepthStencil; 

	public:  
		CClearSceneRendererCommand(CXMLTreeNode &TreeNode); 
		virtual void Execute(CRenderManager &RenderManager); 
};

#endif
	




