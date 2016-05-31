#ifndef _DEFERREDSHADINGSCENERENDERERCOMMAND_H
#define _DEFERREDSHADINGSCENERENDERERCOMMAND_H

#include "StagedTexturedSceneRendererCommand.h"
class CRenderableObjectTechnique;
class CStaticMesh;
class COmniLight;

class CDeferredShadingSceneRendererCommand : public CStagedTexturedSceneRendererCommand 
{ 
	private:  
		CRenderableObjectTechnique *m_RenderableObjectTechnique; 
		ID3D11BlendState *m_EnabledAlphaBlendState; 
		bool m_UseLightVolumes;

		CStaticMesh* m_Sphere; /*Used to optimize deferred shading*/
		bool IsCameraInsideLight(CRenderManager &RenderManager, COmniLight* Light);

		void ExecuteDeferredShading(CRenderManager &RenderManager);
		void ExecuteDeferredShadingUsingLightVolumes(CRenderManager &RenderManager);

	public:  
		CDeferredShadingSceneRendererCommand(CXMLTreeNode &TreeNode);  
		virtual ~CDeferredShadingSceneRendererCommand();  
		void Execute(CRenderManager &RenderManager); 
};

#endif
	

