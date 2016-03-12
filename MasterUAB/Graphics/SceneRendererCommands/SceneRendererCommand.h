#ifndef _SCENERENDERERCOMMAND_H
#define _SCENERENDERERCOMMAND_H

#include "Active.h"
#include "Utils\Named.h"

class CRenderManager;
class CXMLTreeNode;
class CMaterial;

class CSceneRendererCommand : public CActive, public CNamed
{ 
	protected:  
		CMaterial* m_Material;

	public:  
		CSceneRendererCommand(CXMLTreeNode &TreeNode); 
		virtual ~CSceneRendererCommand();  
		virtual void Execute(CRenderManager &RenderManager) = 0; 
}; 

#endif


