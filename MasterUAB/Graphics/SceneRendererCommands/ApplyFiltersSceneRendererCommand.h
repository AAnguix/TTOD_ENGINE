#ifndef _APPLYFILTERSSCENERENDERERCOMMAND_H
#define _APPLYFILTERSSCENERENDERERCOMMAND_H

#include "SceneRendererCommands\StagedTexturedSceneRendererCommand.h"

class CApplyFiltersSceneRendererCommand : public CStagedTexturedSceneRendererCommand
{
	public:
		CApplyFiltersSceneRendererCommand(CXMLTreeNode &TreeNode);
		virtual void Execute(CRenderManager &RenderManager); 
};

#endif