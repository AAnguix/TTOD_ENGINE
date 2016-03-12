#ifndef _CSETPOOLRENDERABLEOBJECTSTECHNIQUESCENERENDERERCOMMAND_H
#define _CSETPOOLRENDERABLEOBJECTSTECHNIQUESCENERENDERERCOMMAND_H

#include "SceneRendererCommand.h"

class CPoolRenderableObjectTechnique;

class CSetPoolRenderableObjectsTechniqueSceneRendererCommand : public CSceneRendererCommand 
{ 

private:  
	CPoolRenderableObjectTechnique *m_Pool;

public: 
	CSetPoolRenderableObjectsTechniqueSceneRendererCommand(CXMLTreeNode &TreeNode);  
	void Execute(CRenderManager &RenderManager);
}; 

#endif