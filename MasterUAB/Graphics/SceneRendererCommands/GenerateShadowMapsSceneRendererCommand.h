#ifndef _GENERATESHADOWMAPSSCENERENDERERCOMMAND_H
#define _GENERATESHADOWMAPSSCENERENDERERCOMMAND_H

#include "SceneRendererCommand.h"

class CGenerateShadowMapsSceneRendererCommand : public CSceneRendererCommand
{
	public:
		CGenerateShadowMapsSceneRendererCommand(CXMLTreeNode &TreeNode);
		virtual ~CGenerateShadowMapsSceneRendererCommand();
		void Execute(CRenderManager &RenderManager);
};

#endif
	




