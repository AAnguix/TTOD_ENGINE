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
	bool m_UseLightVolumes;
	ID3D11DepthStencilState *m_DepthStencilState;

	CStaticMesh* m_SphereFirstPass; /*Used to optimize deferred shading*/
	CStaticMesh* m_SphereSecondPass;

	bool IsCameraInsideLight(CRenderManager &RenderManager, COmniLight* Light);

	void ExecuteDeferredShading(CRenderManager &RenderManager);
	void ExecuteDeferredShadingUsingLightVolumes(CRenderManager &RenderManager);
	void FirstPast(CRenderManager &RenderManager);
	void SecondPass(CRenderManager &RenderManager);

public:  
	CDeferredShadingSceneRendererCommand(CXMLTreeNode &TreeNode);  
	virtual ~CDeferredShadingSceneRendererCommand();  
	void Execute(CRenderManager &RenderManager); 
};

#endif
	

