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

CStaticMesh::CStaticMesh() 
: m_NumVertexs(0)
,m_NumFaces(0)
,m_RVs(NULL)
,m_Materials(NULL)
,CNamed("")
{
	
}

CStaticMesh::~CStaticMesh()
{
	for(size_t i=0;i<m_RVs.size();++i)
	{
		delete m_RVs[i];
	}

	m_RVs.clear();
	m_Materials.clear();
	m_ConvexMeshVertices.clear();
}

bool CStaticMesh::Load(const std::string &FileName)
{
	/*char l_CurrentPath[300];
	_getcwd(l_CurrentPath,sizeof(l_CurrentPath));*/

	std::string l_BinaryFile = FileName;

	FILE *l_File=NULL;
	fopen_s(&l_File, (l_BinaryFile.c_str()),"rb");
	
	m_Name=FileName;

	unsigned short l_DefaultHeader=0xfe55;
	unsigned short l_DefaultFooter=0x55fe;

	unsigned short l_Header;

	fread(&l_Header, sizeof(unsigned short), 1, l_File);

	if(l_Header==l_DefaultHeader)
	{
		unsigned short l_MaterialsCount;
		fread(&l_MaterialsCount, sizeof(unsigned short), 1, l_File);
		
		for(unsigned short i=0;i<l_MaterialsCount;++i)
		{
			unsigned short l_CharactersCount;
			fread(&l_CharactersCount, sizeof(unsigned short), 1, l_File);

			std::string l_MaterialName;
			l_MaterialName.resize(l_CharactersCount+1);
			//fread((void *)l_MaterialName.c_str(), sizeof(l_MaterialName), 1, l_File);
	
			//char * l_MaterialName = new char [l_MaterialName.length()+1];
			//std::strcpy (l_MName, l_MaterialName.c_str());
			fread((void *)l_MaterialName.c_str(), 1, l_CharactersCount+1, l_File);
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

				//Physyx
				long offset = (l_NumBytes / l_NumVertexs) - sizeof(Vect3f);
				void* read = l_VtxsData;

				for (size_t i = 0; i < l_NumVertexs; ++i)
				{
					m_ConvexMeshVertices.push_back(v3fZERO);

					memcpy(&m_ConvexMeshVertices[m_ConvexMeshVertices.size() - 1].x, read, sizeof(float));
					read = static_cast<char*>(read)+4;
					memcpy(&m_ConvexMeshVertices[m_ConvexMeshVertices.size() - 1].y, read, sizeof(float));
					read = static_cast<char*>(read)+4;
					memcpy(&m_ConvexMeshVertices[m_ConvexMeshVertices.size() - 1].z, read, sizeof(float));
					read = static_cast<char*>(read)+4;
					read = static_cast<char*>(read)+offset;
				}

				unsigned short l_IndexType = 0;
				fread(&l_IndexType, sizeof(unsigned short), 1, l_File);

				unsigned int m_NumIndexs;

				if (l_IndexType == 16)
				{
					unsigned short l_NumIndexsFile;
					fread(&l_NumIndexsFile, sizeof(unsigned short), 1, l_File);
					l_NumBytes = sizeof(unsigned short)*l_NumIndexsFile;
					m_NumIndexs = (unsigned int)l_NumIndexsFile;
				}
				else if (l_IndexType == 32)
				{
					unsigned int l_NumIndexsFile;
					fread(&l_NumIndexsFile, sizeof(unsigned int), 1, l_File);
					l_NumBytes = sizeof(unsigned int)*l_NumIndexsFile;
					m_NumIndexs = l_NumIndexsFile;
				}

				l_IdxData = malloc(l_NumBytes);
				fread(l_IdxData, 1, l_NumBytes, l_File);

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

			} // > 0 vertex

			free(l_VtxsData);
			free(l_IdxData);

		} //materials

		m_NumFaces=m_NumVertexs/2;

		unsigned short l_Footer=0;
		fread(&l_Footer, sizeof(unsigned short), 1, l_File);
	
		if(l_Footer!=l_DefaultFooter)
		{
			m_NumFaces=0;
			m_NumVertexs=0;

			for(size_t i=0;i<m_RVs.size();++i)
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
	return Load(m_Name);
}

void CStaticMesh::Render (CRenderManager *RM) const
{
	for(size_t i=0;i<m_RVs.size();++i)
	{
		if(m_Materials[i]!=NULL)
		{
			m_Materials[i]->Apply();
			m_RVs[i]->RenderIndexed(RM, m_Materials[i]->GetRenderableObjectTechnique()->GetEffectTechnique(), (void *)&CEffectManager::m_SceneEffectParameters);
		}
	}
}

CMaterial* CStaticMesh::GetPhysxMaterial()
{
	if(m_Materials.size()>0)
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

float CStaticMesh::GetBoundingSphereRadius() const
{
	return m_BoundingSphere.radius;
}