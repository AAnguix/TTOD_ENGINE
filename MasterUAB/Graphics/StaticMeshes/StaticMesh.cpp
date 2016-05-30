#include "StaticMesh.h"
#include "Render\RenderManager.h"
#include "Materials\MaterialManager.h"
#include "Render\ContextManager.h"
#include "Materials\Material.h"
#include "Vertex\VertexTypes.h"
#include "Engine.h"
#include "Vertex\RenderableVertexs.h"
#include "RenderableObjects\RenderableObjectTechnique.h"
#include <cmath>

CStaticMesh::CStaticMesh(const std::string &Name)
:m_NumVertexs(0)
//,m_NumFaces(0)
,m_RVs(NULL)
,m_Materials(NULL)
,CNamed(Name)
,m_Filename("")
{

}

CStaticMesh::~CStaticMesh()
{
	for (size_t i = 0; i<m_RVs.size(); ++i)
	{
		delete m_RVs[i];
	}

	m_RVs.clear();
	m_Materials.clear();
	m_MeshVertex.clear();
	m_MeshIndex.clear();
}

bool CStaticMesh::Load(const std::string &FileName)
{
	/*char l_CurrentPath[300];
	_getcwd(l_CurrentPath,sizeof(l_CurrentPath));*/

	std::string l_BinaryFile = FileName;

	FILE *l_File = NULL;
	fopen_s(&l_File, (l_BinaryFile.c_str()), "rb");

	m_Filename = FileName;

	unsigned short l_DefaultHeader = 0xfe55;
	unsigned short l_DefaultFooter = 0x55fe;

	unsigned short l_Header;

	fread(&l_Header, sizeof(unsigned short), 1, l_File);

	if (l_Header == l_DefaultHeader)
	{
		unsigned short l_MaterialsCount;
		fread(&l_MaterialsCount, sizeof(unsigned short), 1, l_File);

		for (unsigned short i = 0; i<l_MaterialsCount; ++i)
		{
			unsigned short l_CharactersCount;
			fread(&l_CharactersCount, sizeof(unsigned short), 1, l_File);

			std::string l_MaterialName;
			l_MaterialName.resize(l_CharactersCount + 1);
			//fread((void *)l_MaterialName.c_str(), sizeof(l_MaterialName), 1, l_File);

			//char * l_MaterialName = new char [l_MaterialName.length()+1];
			//std::strcpy (l_MName, l_MaterialName.c_str());
			fread((void *)l_MaterialName.c_str(), 1, l_CharactersCount + 1, l_File);
			l_MaterialName.pop_back();
			m_Materials.push_back(CEngine::GetSingleton().GetMaterialManager()->GetResource(l_MaterialName));
		}

		/*Read B.Box/Sphere*/
		//float l_XMin=0.0f,l_YMin=0.0f,l_ZMin=0.0f,l_XMax=0.0f,l_YMax=0.0f,l_ZMax=0.0f;
		//float l_XCenter=0.0f,l_YCenter=0.0f,l_ZCenter=0.0f,l_Radio=0.0f;

		fread(&m_BoundingBox.x_min, sizeof(float), 1, l_File);
		fread(&m_BoundingBox.y_min, sizeof(float), 1, l_File);
		fread(&m_BoundingBox.z_min, sizeof(float), 1, l_File);

		fread(&m_BoundingBox.x_max, sizeof(float), 1, l_File);
		fread(&m_BoundingBox.y_max, sizeof(float), 1, l_File);
		fread(&m_BoundingBox.z_max, sizeof(float), 1, l_File);

		fread(&m_BoundingSphere.x_center, sizeof(float), 1, l_File);
		fread(&m_BoundingSphere.y_center, sizeof(float), 1, l_File);
		fread(&m_BoundingSphere.z_center, sizeof(float), 1, l_File);
		fread(&m_BoundingSphere.radius, sizeof(float), 1, l_File);

		for (unsigned short i = 0; i < l_MaterialsCount; ++i)
		{
			void *l_VtxsData = NULL;

			void *l_IdxData = NULL;

			unsigned short l_VertexType = 0;
			fread(&l_VertexType, sizeof(unsigned short), 1, l_File);

			unsigned short l_NumVertexs = 0;
			fread(&l_NumVertexs, sizeof(unsigned short), 1, l_File);

			if (l_NumVertexs > 0)
			{
				m_NumVertexs += l_NumVertexs;

				unsigned int l_NumBytes = 0;

				if (l_VertexType == MV_POSITION_WEIGHT_INDICES_NORMAL_TEXTURE_VERTEX::GetVertexType())
					l_NumBytes = sizeof(MV_POSITION_WEIGHT_INDICES_NORMAL_TEXTURE_VERTEX)*l_NumVertexs;

				else if (l_VertexType == MV_POSITION_NORMAL_TEXTURE_VERTEX::GetVertexType())
					l_NumBytes = sizeof(MV_POSITION_NORMAL_TEXTURE_VERTEX)*l_NumVertexs;

				else if (l_VertexType == MV_POSITION4_COLOR_TEXTURE_VERTEX::GetVertexType())
					l_NumBytes = sizeof(MV_POSITION4_COLOR_TEXTURE_VERTEX)*l_NumVertexs;

				else if (l_VertexType == MV_POSITION_COLOR_VERTEX::GetVertexType())
					l_NumBytes = sizeof(MV_POSITION_COLOR_VERTEX)*l_NumVertexs;

				else if (l_VertexType == MV_POSITION_TEXTURE_VERTEX::GetVertexType())
					l_NumBytes = sizeof(MV_POSITION_TEXTURE_VERTEX)*l_NumVertexs;

				else if (l_VertexType == MV_POSITION_COLOR_TEXTURE_VERTEX::GetVertexType())
					l_NumBytes = sizeof(MV_POSITION_COLOR_TEXTURE_VERTEX)*l_NumVertexs;

				else if (l_VertexType == MV_POSITION_NORMAL_TEXTURE_TEXTURE2_VERTEX::GetVertexType())
					l_NumBytes = sizeof(MV_POSITION_NORMAL_TEXTURE_TEXTURE2_VERTEX)*l_NumVertexs;

				else if (l_VertexType == MV_POSITION_NORMAL_TEXTURE_BINORMAL_TANGENT_VERTEX::GetVertexType())
					l_NumBytes = sizeof(MV_POSITION_NORMAL_TEXTURE_BINORMAL_TANGENT_VERTEX)*l_NumVertexs;

				else if (l_VertexType == MV_POSITION_NORMAL_TEXTURE_TEXTURE2_BINORMAL_TANGENT_VERTEX::GetVertexType())
					l_NumBytes = sizeof(MV_POSITION_NORMAL_TEXTURE_TEXTURE2_BINORMAL_TANGENT_VERTEX)*l_NumVertexs;

				l_VtxsData = malloc(l_NumBytes);
				fread(l_VtxsData, l_NumBytes, 1, l_File);

				//Physyx. Storing vertex for cooking meshes
				long offset = (l_NumBytes / l_NumVertexs);
				void* read = l_VtxsData;

				size_t l_LastNumberOfVertices = m_MeshVertex.size();

				m_MeshVertex.resize(l_LastNumberOfVertices + l_NumVertexs);
				for (size_t i = 0; i < l_NumVertexs; ++i)
				{
					Vect3f* l_Vertex = static_cast<Vect3f*>(read);
					m_MeshVertex[i + l_LastNumberOfVertices] = *l_Vertex;

					read = static_cast<char*>(read)+offset;
				}
				//

				unsigned short l_IndexType = 0;
				fread(&l_IndexType, sizeof(unsigned short), 1, l_File);

				unsigned short m_NumIndexs;

				bool l_32bits = false;

				if (l_IndexType == 16)
				{
					unsigned short l_NumIndexsFile;
					fread(&l_NumIndexsFile, sizeof(unsigned short), 1, l_File);
					l_NumBytes = sizeof(unsigned short)*l_NumIndexsFile;
					m_NumIndexs = (unsigned short)l_NumIndexsFile;
				}
				else if (l_IndexType == 32)
				{
					l_32bits = true;
					unsigned int l_NumIndexsFile;
					fread(&l_NumIndexsFile, sizeof(unsigned int), 1, l_File);
					l_NumBytes = sizeof(unsigned int)*l_NumIndexsFile;
					m_NumIndexs = l_NumIndexsFile;
				}

				l_IdxData = malloc(l_NumBytes);
				fread(l_IdxData, 1, l_NumBytes, l_File);

				/*Index for cooking triangle meshes*/
				void* l_ReadIndex = l_IdxData;
				//m_MeshIndex.resize(m_NumIndexs);
				//memcpy(&m_MeshIndex[0], l_ReadIndex, sizeof(unsigned short)* m_NumIndexs);

				size_t l_LastNumberOfIndexs = m_MeshIndex.size();
				m_MeshIndex.resize(l_LastNumberOfIndexs + m_NumIndexs);
				for (size_t i = 0; i < m_NumIndexs; ++i)
				{
					unsigned short* l_Index = static_cast<unsigned short*>(l_ReadIndex);
					m_MeshIndex[i + l_LastNumberOfIndexs] = *l_Index + l_LastNumberOfVertices;
					//m_MeshIndex[i + l_LastNumberOfIndexs] = *l_Index;
					l_ReadIndex = static_cast<char*>(l_ReadIndex)+sizeof(unsigned short);
				}

				/*end*/

				CRenderableVertexs *l_RV = NULL;

				if (l_VertexType == MV_POSITION_WEIGHT_INDICES_NORMAL_TEXTURE_VERTEX::GetVertexType())
				{
					if (l_IndexType == 16)
					{
						l_RV = new CTriangleListRenderableIndexed16Vertexs<MV_POSITION_WEIGHT_INDICES_NORMAL_TEXTURE_VERTEX>(l_VtxsData, l_NumVertexs, l_IdxData, m_NumIndexs, false);
					}
					else
					{
						l_RV = new CTriangleListRenderableIndexed32Vertexs<MV_POSITION_WEIGHT_INDICES_NORMAL_TEXTURE_VERTEX>(l_VtxsData, l_NumVertexs, l_IdxData, m_NumIndexs, false);
					}
				}

				else if (l_VertexType == MV_POSITION_NORMAL_TEXTURE_VERTEX::GetVertexType())
				{
					if (l_IndexType == 16)
					{
						l_RV = new CTriangleListRenderableIndexed16Vertexs<MV_POSITION_NORMAL_TEXTURE_VERTEX>(l_VtxsData, l_NumVertexs, l_IdxData, m_NumIndexs, false);
					}
					else
					{
						l_RV = new CTriangleListRenderableIndexed32Vertexs<MV_POSITION_NORMAL_TEXTURE_VERTEX>(l_VtxsData, l_NumVertexs, l_IdxData, m_NumIndexs, false);
					}
				}

				else if (l_VertexType == MV_POSITION4_COLOR_TEXTURE_VERTEX::GetVertexType())
				{
					if (l_IndexType == 16)
					{
						l_RV = new CTriangleListRenderableIndexed16Vertexs<MV_POSITION4_COLOR_TEXTURE_VERTEX>(l_VtxsData, l_NumVertexs, l_IdxData, m_NumIndexs, false);
					}
					else
					{
						l_RV = new CTriangleListRenderableIndexed32Vertexs<MV_POSITION4_COLOR_TEXTURE_VERTEX>(l_VtxsData, l_NumVertexs, l_IdxData, m_NumIndexs, false);
					}
				}

				else if (l_VertexType == MV_POSITION_COLOR_VERTEX::GetVertexType())
				{
					if (l_IndexType == 16)
					{
						l_RV = new CTriangleListRenderableIndexed16Vertexs<MV_POSITION_COLOR_VERTEX>(l_VtxsData, l_NumVertexs, l_IdxData, m_NumIndexs, false);
					}
					else
					{
						l_RV = new CTriangleListRenderableIndexed32Vertexs<MV_POSITION_COLOR_VERTEX>(l_VtxsData, l_NumVertexs, l_IdxData, m_NumIndexs, false);
					}
				}

				else if (l_VertexType == MV_POSITION_TEXTURE_VERTEX::GetVertexType())
				{
					if (l_IndexType == 16)
					{
						l_RV = new CTriangleListRenderableIndexed16Vertexs<MV_POSITION_TEXTURE_VERTEX>(l_VtxsData, l_NumVertexs, l_IdxData, m_NumIndexs, false);
					}
					else
					{
						l_RV = new CTriangleListRenderableIndexed32Vertexs<MV_POSITION_TEXTURE_VERTEX>(l_VtxsData, l_NumVertexs, l_IdxData, m_NumIndexs, false);
					}
				}

				else if (l_VertexType == MV_POSITION_COLOR_TEXTURE_VERTEX::GetVertexType())
				{
					if (l_IndexType == 16)
					{
						l_RV = new CTriangleListRenderableIndexed16Vertexs<MV_POSITION_COLOR_TEXTURE_VERTEX>(l_VtxsData, l_NumVertexs, l_IdxData, m_NumIndexs, false);
					}
					else
					{
						l_RV = new CTriangleListRenderableIndexed32Vertexs<MV_POSITION_COLOR_TEXTURE_VERTEX>(l_VtxsData, l_NumVertexs, l_IdxData, m_NumIndexs, false);
					}
				}
				//News
				else if (l_VertexType == MV_POSITION_NORMAL_TEXTURE_TEXTURE2_VERTEX::GetVertexType())
				{
					if (l_IndexType == 16)
					{
						l_RV = new CTriangleListRenderableIndexed16Vertexs<MV_POSITION_NORMAL_TEXTURE_TEXTURE2_VERTEX>(l_VtxsData, l_NumVertexs, l_IdxData, m_NumIndexs, false);
					}
					else
					{
						l_RV = new CTriangleListRenderableIndexed32Vertexs<MV_POSITION_NORMAL_TEXTURE_TEXTURE2_VERTEX>(l_VtxsData, l_NumVertexs, l_IdxData, m_NumIndexs, false);
					}
				}
				else if (l_VertexType == MV_POSITION_NORMAL_TEXTURE_BINORMAL_TANGENT_VERTEX::GetVertexType())
				{
					CRenderableVertexs::CalcTangentsAndBinormals(l_VtxsData, (unsigned short*)l_IdxData, l_NumVertexs, m_NumIndexs, sizeof(MV_POSITION_NORMAL_TEXTURE_BINORMAL_TANGENT_VERTEX), 0, 12, 24, 40, 56);

					if (l_IndexType == 16)
					{
						l_RV = new CTriangleListRenderableIndexed16Vertexs<MV_POSITION_NORMAL_TEXTURE_BINORMAL_TANGENT_VERTEX>(l_VtxsData, l_NumVertexs, l_IdxData, m_NumIndexs, false);
					}
					else
					{
						l_RV = new CTriangleListRenderableIndexed32Vertexs<MV_POSITION_NORMAL_TEXTURE_BINORMAL_TANGENT_VERTEX>(l_VtxsData, l_NumVertexs, l_IdxData, m_NumIndexs, false);
					}
				}

				else if (l_VertexType == MV_POSITION_NORMAL_TEXTURE_TEXTURE2_BINORMAL_TANGENT_VERTEX::GetVertexType()) //Pos,Normal,UV,Bi,Tang
				{																											// Pos, N, T, Bi, UV
					CRenderableVertexs::CalcTangentsAndBinormals(l_VtxsData, (unsigned short*)l_IdxData, l_NumVertexs, m_NumIndexs, sizeof(MV_POSITION_NORMAL_TEXTURE_TEXTURE2_BINORMAL_TANGENT_VERTEX), 0, 12, 24, 40, 56);

					if (l_IndexType == 16)
					{
						l_RV = new CTriangleListRenderableIndexed16Vertexs<MV_POSITION_NORMAL_TEXTURE_TEXTURE2_BINORMAL_TANGENT_VERTEX>(l_VtxsData, l_NumVertexs, l_IdxData, m_NumIndexs, false);
					}
					else
					{
						l_RV = new CTriangleListRenderableIndexed32Vertexs<MV_POSITION_NORMAL_TEXTURE_TEXTURE2_BINORMAL_TANGENT_VERTEX>(l_VtxsData, l_NumVertexs, l_IdxData, m_NumIndexs, false);
					}
				}

				m_RVs.push_back(l_RV);

				//m_NumFaces += (m_NumIndexs - 2);

			} // > 0 vertex

			free(l_VtxsData);
			free(l_IdxData);

		} //materials

		unsigned short l_Footer = 0;
		fread(&l_Footer, sizeof(unsigned short), 1, l_File);

		if (l_Footer != l_DefaultFooter)
		{
			//m_NumFaces = 0;
			m_NumVertexs = 0;

			for (size_t i = 0; i<m_RVs.size(); ++i)
			{
				delete m_RVs[i];
			}

			m_RVs.clear();

			m_Materials.clear();
		}

	}//check header

	fclose(l_File);

	return 0;
}

bool CStaticMesh::Reload()
{
	return Load(m_Filename);
}

void CStaticMesh::Render(CRenderManager *RM) const
{
	for (size_t i = 0; i<m_RVs.size(); ++i)
	{
		if (m_Materials[i] != NULL)
		{
			m_Materials[i]->Apply();
			m_RVs[i]->RenderIndexed(RM, m_Materials[i]->GetRenderableObjectTechnique()->GetEffectTechnique(), (void *)&CEffectManager::m_SceneEffectParameters);
		}
	}
}

CMaterial* CStaticMesh::GetPhysxMaterial()
{
	if (m_Materials.size()>0)
		return m_Materials[0];

	return nullptr;
}

Vect3f CStaticMesh::GetBoundingBoxSize() const
{
	Vect3f l_Size;
	l_Size.x = abs(m_BoundingBox.x_max - m_BoundingBox.x_min);
	l_Size.y = abs(m_BoundingBox.y_max - m_BoundingBox.y_min);
	l_Size.z = abs(m_BoundingBox.z_max - m_BoundingBox.z_min);
	return l_Size;
}

Vect3f CStaticMesh::GetBoundingBoxMax() const
{
	return Vect3f(m_BoundingBox.x_max, m_BoundingBox.y_max, m_BoundingBox.z_max);
}

Vect3f CStaticMesh::GetBoundingBoxMin() const
{
	return Vect3f(m_BoundingBox.x_min, m_BoundingBox.y_min, m_BoundingBox.z_min);
}

Vect3f CStaticMesh::GetBoundingSphereCenter() const
{
	return Vect3f(m_BoundingSphere.x_center, m_BoundingSphere.y_center, m_BoundingSphere.z_center);
}

float CStaticMesh::GetBoundingSphereRadius() const
{
	return m_BoundingSphere.radius;
}

float CStaticMesh::GetCapsuleHalfHeight() const
{
	float l_Result = abs(m_BoundingBox.y_max - m_BoundingBox.y_min) / 2;
	return l_Result;
}

float CStaticMesh::GetCapsuleRadius() const
{
	float l_Result = (abs(m_BoundingBox.x_max - m_BoundingBox.x_min)) / 2;
	return l_Result;
}

CEmptyPointerClass* CStaticMesh::GetThisLuaAddress() const { return (CEmptyPointerClass *)((void*)this); }