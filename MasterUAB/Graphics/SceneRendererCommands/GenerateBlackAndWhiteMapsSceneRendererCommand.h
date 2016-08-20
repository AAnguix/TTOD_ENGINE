#ifndef _GENERATEBLACKANDWHITEMAPSSCENERENDERERCOMMAND_H
#define _GENERATEBLACKANDWHITEMAPSSCENERENDERERCOMMAND_H

#include "SceneRendererCommand.h"
#include "StagedTexturedSceneRendererCommand.h"

class CGenerateBlackAndWhiteMapsSceneRendererCommand : public CStagedTexturedSceneRendererCommand
{
	private:
		CRenderableObjectTechnique *m_RenderableObjectTechnique;

	public:
		CGenerateBlackAndWhiteMapsSceneRendererCommand(CXMLTreeNode &TreeNode);
		virtual ~CGenerateBlackAndWhiteMapsSceneRendererCommand();
		void Execute(CRenderManager &RenderManager);
};

#endif
	




