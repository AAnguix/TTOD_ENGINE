#ifndef _DISABLEALPHABLENDSCENERENDERERCOMMAND_H
#define _DISABLEALPHABLENDSCENERENDERERCOMMAND_H

#include "SceneRendererCommand.h"

class CDisableAlphaBlendSceneRendererCommand : public CSceneRendererCommand
{
	public:
		CDisableAlphaBlendSceneRendererCommand(CXMLTreeNode &TreeNode);
		virtual void Execute(CRenderManager &RenderManager);
};

#endif
	




