#ifndef _RENDERDEBUGGRIDSCENERENDERERCOMMAND_H
#define _RENDERDEBUGGRIDSCENERENDERERCOMMAND_H

#include "SceneRendererCommand.h"

class CRenderDebugGridSceneRendererCommand : public CSceneRendererCommand
{
	public:
	CRenderDebugGridSceneRendererCommand(CXMLTreeNode &TreeNode);
	void Execute(CRenderManager &RenderManager);
};

#endif
	




