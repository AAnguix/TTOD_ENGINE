#ifndef _ENABLEADDITIVEALPHABLENDSCENERENDERERCOMMAND_H
#define _ENABLEADDITIVEALPHABLENDSCENERENDERERCOMMAND_H

#include "SceneRendererCommand.h"
#include <D3D11.h>

class CEnableAdditiveAlphaBlendSceneRendererCommand : public CSceneRendererCommand
{
private:
	ID3D11BlendState    *m_AdditiveAlphaBlendState;

public:
	CEnableAdditiveAlphaBlendSceneRendererCommand(CXMLTreeNode &TreeNode);
	virtual void Execute(CRenderManager &RenderManager);
};

#endif
	




