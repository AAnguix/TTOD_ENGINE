#ifndef _STATICMESH_H
#define _STATICMESH_H

#include <vector>
#include "Utils\Named.h"

class CRenderableVertexs;
class CRenderManager;
class CMaterial;

struct AxisAlignedBoundingBox
{
	float center;
	float x_min,y_min,z_min;
	float x_max,y_max,z_max;
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
	AxisAlignedBoundingBox m_BoundingBox;
	AxisAlignedBoundingSphere m_BoundingSphere;

	std::vector<CRenderableVertexs*> m_RVs;
	std::vector<CMaterial *> m_Materials;
	std::vector<Vect3f> m_ConvexMeshVertices;
	unsigned int m_NumVertexs, m_NumFaces;
	
public:
	CStaticMesh();
	~CStaticMesh();
	bool Load (const std::string &FileName);
	bool Reload ();
	void Render (CRenderManager *RM) const;

	CMaterial* GetPhysxMaterial();
	Vect3f GetBoundingBoxSize() const;
	float GetBoundingSphereRadius() const;
	std::vector<Vect3f> GetConvexMeshVertices() const{return m_ConvexMeshVertices;};
};

#endif 