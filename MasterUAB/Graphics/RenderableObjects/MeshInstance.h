#ifndef _MESHINSTANCE_H
#define _MESHINSTANCE_H

#include "RenderableObjects\RenderableObject.h"
class CGameObject;
class CStaticMesh;
class CXMLTreeNode;
class CAnimatedInstanceModel;
class CRenderableObjectTechnique;

class CMeshInstance : public CRenderableObject 
{  

private:   
	CStaticMesh *m_StaticMesh;  
	CRenderableObjectTechnique* m_TemporalRenderbleObjectTechnique;

	int m_ParentBoneId;
	CAnimatedInstanceModel* m_Parent;

public:   
	CMeshInstance(CGameObject* Owner, const std::string &Name, const std::string &CoreName, const Vect3f &Position, float Yaw, float Pitch, float Roll);
	CMeshInstance(CXMLTreeNode &TreeNode);   
	~CMeshInstance();  

	void GeneratePhysxActor(CXMLTreeNode &TreeNode);
	void Render(CRenderManager* RenderManager);
	void SetTemporalRenderableObjectTechnique(CRenderableObjectTechnique* RenderableObjectTechnique);
	TRenderableObjectType GetClassType() const{ return TRenderableObjectType::MESH_INSTANCE; };

	CAnimatedInstanceModel* GetParent() const { return m_Parent; };
	int GetParentBoneID() const { return m_ParentBoneId; };
	void SetParent(CAnimatedInstanceModel* Parent, const std::string &BoneName);
	CStaticMesh* GetStaticMesh() const{ return m_StaticMesh; };
};

#endif
