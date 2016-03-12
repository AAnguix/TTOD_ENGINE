#ifndef _RENDERGUISCENERENDERERCOMMAND_H
#define _RENDERGUISCENERENDERERCOMMAND_H

#include "SceneRendererCommand.h"

class CRenderGUISceneRendererCommand : public CSceneRendererCommand 
{ 
	private:  
		//TODO CGUI   *m_GUI; -FALTA HACERLO
	public:  
		CRenderGUISceneRendererCommand(CXMLTreeNode &TreeNode);  
		virtual void Execute(CRenderManager &RenderManager); 
};

#endif
