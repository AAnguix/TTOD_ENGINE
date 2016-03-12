#ifndef _RENDERDEBUGSHADOWMAPSSCENERENDERERCOMMAND_H
#define _RENDERDEBUGSHADOWMAPSSCENERENDERERCOMMAND_H
#include "SceneRendererCommand.h"
class CRenderableObjectTechnique;

class CRenderDebugShadowMapsSceneRendererCommand : public CSceneRendererCommand
{
	private:
		float m_Width;
		float m_Offset;
		CRenderableObjectTechnique *m_RenderableObjectTechnique;
	public:
		CRenderDebugShadowMapsSceneRendererCommand(CXMLTreeNode &TreeNode);
		void Execute(CRenderManager &RenderManager);
};

#endif
	




