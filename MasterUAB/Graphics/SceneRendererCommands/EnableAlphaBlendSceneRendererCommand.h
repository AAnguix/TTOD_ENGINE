#ifndef _ENABLEALPHABLENDSCENERENDERERCOMMAND_H
#define _ENABLEALPHABLENDSCENERENDERERCOMMAND_H

#include "SceneRendererCommand.h"

class CEnableAlphaBlendSceneRendererCommand : public CSceneRendererCommand
{

public:
	CEnableAlphaBlendSceneRendererCommand(CXMLTreeNode &TreeNode);
	virtual void Execute(CRenderManager &RenderManager);
};

#endif
	




