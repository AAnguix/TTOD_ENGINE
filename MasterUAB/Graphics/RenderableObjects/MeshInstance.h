#ifndef _MESHINSTANCE_H
#define _MESHINSTANCE_H

#include "RenderableObjects\RenderableObject.h"
class CStaticMesh;
class CXMLTreeNode;
class CAnimatedInstanceModel;
class CCollider;

class CMeshInstance : public CRenderableObject 
{  

private:   
	CStaticMesh *m_StaticMesh;  
	CCollider *m_Collider;

	int m_ParentBoneId;
	CAnimatedInstanceModel* m_Parent;

public:   
	CMeshInstance(const std::string &Name, const std::string &CoreName, const Vect3f &Position, float Yaw, float Pitch, float Roll);
	CMeshInstance(CXMLTreeNode &TreeNode);   
	~CMeshInstance();  
	void Render(CRenderManager* RenderManager);
	TRenderableObjectType GetClassType() const{ return TRenderableObjectType::MESH_INSTANCE; };

	void SetParent(CAnimatedInstanceModel* Parent, const std::string &BoneName);
	CStaticMesh* GetStaticMesh() const{ return m_StaticMesh; };

	virtual CCollider* GetCollider() const;
	virtual void SetCollider(CCollider* CCollider);
};

#endif
