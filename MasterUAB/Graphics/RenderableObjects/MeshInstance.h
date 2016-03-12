#ifndef _MESHINSTANCE_H
#define _MESHINSTANCE_H

#include "RenderableObjects\RenderableObject.h"
class CStaticMesh;
class CXMLTreeNode;

class CMeshInstance : public CRenderableObject 
{  

private:   
	CStaticMesh   *m_StaticMesh;  

public:   
	CMeshInstance(const std::string &Name, const std::string &CoreName);   
	CMeshInstance(CXMLTreeNode &TreeNode);   
	~CMeshInstance();  
	void Render(CRenderManager* RenderManager); 
	TRenderableObjectType GetClassType() const{ return TRenderableObjectType::MESH_INSTANCE; };
};

#endif
