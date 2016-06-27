#ifndef _CAPTURESTENCILBUFFERSCENERENDERERCOMMAND_H
#define _CAPTURESTENCILBUFFERSCENERENDERERCOMMAND_H

#include "StagedTexturedSceneRendererCommand.h"

class CCaptureStencilBufferSceneRendererCommand : public CStagedTexturedSceneRendererCommand
{
public:
	CCaptureStencilBufferSceneRendererCommand(CXMLTreeNode &TreeNode);
	virtual void Execute(CRenderManager &RenderManager);
};

#endif






