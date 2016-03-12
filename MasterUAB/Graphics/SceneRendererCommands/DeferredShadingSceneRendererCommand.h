#ifndef _DEFERREDSHADINGSCENERENDERERCOMMAND_H
#define _DEFERREDSHADINGSCENERENDERERCOMMAND_H

#include "StagedTexturedSceneRendererCommand.h"
class CRenderableObjectTechnique;

class CDeferredShadingSceneRendererCommand : public CStagedTexturedSceneRendererCommand 
{ 
	private:  
		CRenderableObjectTechnique   *m_RenderableObjectTechnique; 
		ID3D11BlendState    *m_EnabledAlphaBlendState; 
	public:  
		CDeferredShadingSceneRendererCommand(CXMLTreeNode &TreeNode);  
		virtual ~CDeferredShadingSceneRendererCommand();  
		void Execute(CRenderManager &RenderManager); 
};

#endif
	

