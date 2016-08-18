#ifndef _STATICMESH_H
#define _STATICMESH_H

#include <vector>
#include "Utils\Named.h"
#include "Math\Vector3.h"

class CRenderableVertexs;
class CRenderManager;
class CMaterial;
class CRenderableObjectTechnique;

namespace physx
{
	class PxShape;
	class PxGeometry;
}

struct AxisAlignedBoundingBox
{
	float center;
	float x_min, y_min, z_min;
	float x_max, y_max, z_max;
};

struct AxisAlignedBoundingSphere
{
	float x_center;
	float y_center;
	float z_center;
	float radius;
};

class CXMLTreeNode;

class CStaticMesh : public CNamed
{

protected:
	std::string m_Filename;
	AxisAlignedBoundingBox m_BoundingBox;
	AxisAlignedBoundingSphere m_BoundingSphere;

	std::vector<CRenderableVertexs*> m_RVs;
	std::vector<CMaterial *> m_Materials;
	physx::PxShape* m_Shape;
	unsigned int m_NumVertexs;

	std::vector<Vect3f> m_MeshVertex;
	std::vector<unsigned short> m_MeshIndex;

public:
	//unsigned int  m_NumFaces;
	CStaticMesh(const CXMLTreeNode &XMLTreeNode);
	~CStaticMesh();
	bool Load(const std::string &FileName);
	bool LoadShape(const std::string &ShapeType, const std::string &Group, bool IsExclusive);

	bool Reload();
	void Render(CRenderManager *RM, CRenderableObjectTechnique* Technique = NULL) const;

	CMaterial* GetPhysxMaterial();
	
	Vect3f GetBoundingBoxSize() const;
	Vect3f GetBoundingBoxMax() const;
	Vect3f GetBoundingBoxMin() const;

	Vect3f GetBoundingSphereCenter() const;
	float GetBoundingSphereRadius() const;
	
	float GetCapsuleHalfHeight() const;
	float GetCapsuleRadius() const;
	
	std::vector<Vect3f> GetVertex() const{ return m_MeshVertex; };
	std::vector<unsigned short> GetIndex() const{ return m_MeshIndex; };

	CEmptyPointerClass* GetThisLuaAddress() const;
};

#endif 