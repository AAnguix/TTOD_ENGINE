#ifndef _RENDERDEBUGBLACKANDWHITEMAPSSCENERENDERERCOMMAND_H
#define _RENDERDEBUGBLACKANDWHITEMAPSSCENERENDERERCOMMAND_H 
#include "SceneRendererCommand.h"
class CRenderableObjectTechnique;

class CRenderDebugBlackAndWhiteMapsSceneRendererCommand : public CSceneRendererCommand
{
	private:
		float m_Width;
		float m_Offset;
		bool m_Blurred;
		CRenderableObjectTechnique *m_RenderableObjectTechnique;
	public:
		CRenderDebugBlackAndWhiteMapsSceneRendererCommand(CXMLTreeNode &TreeNode);
		void Execute(CRenderManager &RenderManager);
};

#endif
	




