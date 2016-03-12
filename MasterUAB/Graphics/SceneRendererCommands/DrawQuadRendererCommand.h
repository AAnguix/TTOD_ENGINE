#ifndef _DRAWQUADRENDERERCOMMAND_H
#define _DRAWQUADRENDERERCOMMAND_H

class CRenderManager;
class CXMLTreeNode;
class CRenderableObjectTechnique;
#include "StagedTexturedSceneRendererCommand.h"

class CDrawQuadRendererCommand : public CStagedTexturedSceneRendererCommand 
{ 
	private:  
		CRenderableObjectTechnique	*m_RenderableObjectTechnique; 
		
	public:  
		CDrawQuadRendererCommand(CXMLTreeNode &TreeNode);  
		void Execute(CRenderManager &RenderManager); 
};

#endif
	
