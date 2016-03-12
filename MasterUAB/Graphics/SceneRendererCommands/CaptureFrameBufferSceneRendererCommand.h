#ifndef _CAPTUREFRAMEBUFFERSCENERENDERERCOMMAND_H
#define _CAPTUREFRAMEBUFFERSCENERENDERERCOMMAND_H

#include "StagedTexturedSceneRendererCommand.h"

class CCaptureFrameBufferSceneRendererCommand : public CStagedTexturedSceneRendererCommand
{
	public:
		CCaptureFrameBufferSceneRendererCommand(CXMLTreeNode &TreeNode);
		virtual void Execute(CRenderManager &RenderManager);
};

#endif
	




