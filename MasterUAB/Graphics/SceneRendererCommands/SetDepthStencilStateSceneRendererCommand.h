#ifndef _SETDEPTHSTENCILSTATESCENERENDERERCOMMAND_H
#define _SETDEPTHSTENCILSTATESCENERENDERERCOMMAND_H

#include "SceneRendererCommand.h"
#include <D3D11.h>

class CSetDepthStencilStateSceneRendererCommand : public CSceneRendererCommand 
{ 
	private:  
		ID3D11DepthStencilState   *m_DepthStencilState; 
	
	public:  
		CSetDepthStencilStateSceneRendererCommand(CXMLTreeNode &TreeNode);  
		virtual ~CSetDepthStencilStateSceneRendererCommand();  
		void Execute(CRenderManager &RenderManager); 
}; 

#endif

	




