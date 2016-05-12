#include "Engine.h"
#include "Render\DebugRender.h"
#include "Render\ContextManager.h"
#include "Vertex\VertexTypes.h"
#include "Effects\Effect.h"

#include "Lights\LightManager.h"
#include "Lights\SpotLight.h"

#include "RenderableObjects\RenderableObjectsManager.h"

CDebugRender::CDebugRender()
{
	{
		//AXIS
		MV_POSITION_COLOR_VERTEX l_AxisVtxs[6] =
		{
			{ Vect3f(0.0f, 0.0f, 0.0f), CColor(1.0f, 0.0f, 0.0f, 1.0f) },
			{ Vect3f(1.0f, 0.0f, 0.0f), CColor(1.0f, 0.0f, 0.0f, 1.0f) },

			{ Vect3f(0.0f, 0.0f, 0.0f), CColor(0.0f, 1.0f, 0.0f, 1.0f) },
			{ Vect3f(0.0f, 1.0f, 0.0f), CColor(0.0f, 1.0f, 0.0f, 1.0f) },

			{ Vect3f(0.0f, 0.0f, 0.0f), CColor(0.0f, 0.0f, 1.0f, 1.0f) },
			{ Vect3f(0.0f, 0.0f, 1.0f), CColor(0.0f, 0.0f, 1.0f, 1.0f) }
		};

		m_Axis = new CLinesListRenderableVertexs<MV_POSITION_COLOR_VERTEX>(l_AxisVtxs, 6, 3, false);
		m_AxisBSRadi = 1;
		m_AxisBBMin = Vect3f(0, 0, 0);
		m_AxisBBMax = Vect3f(1, 1, 1);
	}
	{
		MV_POSITION_COLOR_VERTEX l_LongAxisVtxs[12] =
		{
			{ Vect3f(0.0f, 0.0f, 0.0f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(1.0f, 0.0f, 0.0f), CColor(1.0f, 0.0f, 0.0f, 1.0f) },

			{ Vect3f(0.0f, 0.0f, 0.0f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(0.0f, 1.0f, 0.0f), CColor(0.0f, 1.0f, 0.0f, 1.0f) },

			{ Vect3f(0.0f, 0.0f, 0.0f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(0.0f, 0.0f, 1.0f), CColor(0.0f, 0.0f, 1.0f, 1.0f) },

			{ Vect3f(0.0f, 0.0f, 0.0f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(-1.0f, 0.0f, 0.0f), CColor(0.0f, 1.0f, 1.0f, 1.0f) },

			{ Vect3f(0.0f, 0.0f, 0.0f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(0.0f, -1.0f, 0.0f), CColor(1.0f, 0.0f, 1.0f, 1.0f) },

			{ Vect3f(0.0f, 0.0f, 0.0f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
			{ Vect3f(0.0f, 0.0f, -1.0f), CColor(1.0f, 1.0f, 0.0f, 1.0f) }
		};

		m_LongAxis = new CLinesListRenderableVertexs<MV_POSITION_COLOR_VERTEX>(l_LongAxisVtxs, 12, 6, false);
		m_LongAxisBSRadi = 1;
		m_LongAxisBBMin = Vect3f(-1, -1, -1);
		m_LongAxisBBMax = Vect3f(1, 1, 1);
	}
	{
		//SIMPLE TRIANGLE
		MV_POSITION_COLOR_VERTEX l_SimpleTriangleVtxs[3] =
		{
			{ Vect3f(-0.5f, -0.5f, 0.5f), CColor(1.0f, 0.0f, 0.0f, 1.0f) },
			{ Vect3f(+0.0f, +0.5f, 0.5f), CColor(0.0f, 1.0f, 0.0f, 1.0f) },
			{ Vect3f(+0.5f, -0.5f, 0.5f), CColor(0.0f, 0.0f, 1.0f, 1.0f) },
		};

		m_SimpleTriangle = new CTrianglesListRenderableVertexs<MV_POSITION_COLOR_VERTEX>(l_SimpleTriangleVtxs, 3, 1, false);
		m_SimpleTriangleBSRadi = 0.86603f;
		m_SimpleTriangleBBMin = Vect3f(-0.5f, -0.5f, 0.5f);
		m_SimpleTriangleBBMax = Vect3f(0.5f, 0.5f, 0.5f);
	}
	{
		//SIMPLE TRIANGLE
		MV_POSITION_COLOR_VERTEX l_SimpleTriangleVtxs[3] =
		{
			{ Vect3f(-0.5f, -0.5f, 0.5f), CColor(1.0f, 0.0f, 0.0f, 0.5f) },
			{ Vect3f(+0.0f, +0.5f, 0.5f), CColor(0.0f, 1.0f, 0.0f, 0.5f) },
			{ Vect3f(+0.5f, -0.5f, 0.5f), CColor(0.0f, 0.0f, 1.0f, 0.5f) },
		};

		m_ClassicBlendTriangle = new CTrianglesListRenderableVertexs<MV_POSITION_COLOR_VERTEX>(l_SimpleTriangleVtxs, 3, 1, false);
		m_ClassicBlendTriangleBSRadi = 0.86603f;
		m_ClassicBlendTriangleBBMin = Vect3f(-0.5f, -0.5f, 0.5f);
		m_ClassicBlendTriangleBBMax = Vect3f(0.5f, 0.5f, 0.5f);
	}
	{
		//SIMPLE TRIANGLE
		MV_POSITION_COLOR_VERTEX l_SimpleTriangleVtxs[3] =
		{
			{ Vect3f(-0.5f, -0.5f, 0.5f), CColor(0.5f, 0.0f, 0.0f, 0.5f) },
			{ Vect3f(+0.0f, +0.5f, 0.5f), CColor(0.0f, 0.5f, 0.0f, 0.5f) },
			{ Vect3f(+0.5f, -0.5f, 0.5f), CColor(0.0f, 0.0f, 0.5f, 0.5f) },
		};

		m_PremultBlendTriangle = new CTrianglesListRenderableVertexs<MV_POSITION_COLOR_VERTEX>(l_SimpleTriangleVtxs, 3, 1, false);
		m_PremultBlendTriangleBSRadi = 0.86603f;
		m_PremultBlendTriangleBBMin = Vect3f(-0.5f, -0.5f, 0.5f);
		m_PremultBlendTriangleBBMax = Vect3f(0.5f, 0.5f, 0.5f);
	}
	{
		// Simple Grid
		MV_POSITION_COLOR_VERTEX l_SimpleGridVtxs[] =
		{
			{ Vect3f(+1.0f, 0.0f, +1.0f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },   // 0
			{ Vect3f(+1.0f, 0.0f, -1.0f), CColor(1.0f, 1.0f, 0.0f, 1.0f) },  // 1
			{ Vect3f(-1.0f, 0.0f, -1.0f), CColor(1.0f, 0.0f, 0.0f, 1.0f) }, // 2
			{ Vect3f(-1.0f, 0.0f, +1.0f), CColor(1.0f, 0.0f, 1.0f, 1.0f) },  // 3
			{ Vect3f(+1.0f, 0.0f, +0.0f), CColor(1.0f, 1.0f, 0.5f, 1.0f) },   // 4
			{ Vect3f(-1.0f, 0.0f, +0.0f), CColor(1.0f, 0.0f, 0.5f, 1.0f) },  // 5
			{ Vect3f(+0.0f, 0.0f, +1.0f), CColor(1.0f, 0.5f, 1.0f, 1.0f) },   // 6
			{ Vect3f(+0.0f, 0.0f, -1.0f), CColor(1.0f, 0.5f, 0.0f, 1.0f) },  // 7
		};

		uint16_t l_SimpleGridIdxs[] = { 0, 1, 0, 3, 2, 1, 2, 3, 4, 5, 6, 7 };

		m_SimpleGrid = new CLinesListRenderableIndexed16Vertexs<MV_POSITION_COLOR_VERTEX>(l_SimpleGridVtxs, 8, l_SimpleGridIdxs, 12, false);
		m_SimpleGridBSRadi = 1.414236f;
		m_SimpleGridBBMin = Vect3f(-1.f, 0.f, -1.f);
		m_SimpleGridBBMax = Vect3f(1.f, 0.f, 1.f);
	}
	{
		// Simple Cube
		MV_POSITION_COLOR_VERTEX l_SimpleCubeVtxs[] =
		{
			{ Vect3f(+0.5f, 0.0f, +0.5f), CColor(0.0f, 1.0f, 1.0f, 1.0f) },   // 0
			{ Vect3f(+0.5f, 0.0f, -0.5f), CColor(0.0f, 1.0f, 0.0f, 1.0f) },  // 1
			{ Vect3f(-0.5f, 0.0f, -0.5f), CColor(0.0f, 0.0f, 0.0f, 1.0f) }, // 2
			{ Vect3f(-0.5f, 0.0f, +0.5f), CColor(0.0f, 0.0f, 1.0f, 1.0f) },  // 3
			{ Vect3f(+0.5f, 1.0f, +0.5f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },   // 4
			{ Vect3f(+0.5f, 1.0f, -0.5f), CColor(1.0f, 1.0f, 0.0f, 1.0f) },  // 5
			{ Vect3f(-0.5f, 1.0f, -0.5f), CColor(1.0f, 0.0f, 0.0f, 1.0f) },   // 6
			{ Vect3f(-0.5f, 1.0f, +0.5f), CColor(1.0f, 0.0f, 1.0f, 1.0f) },  // 7
		};

		/*
		uint16_t l_SimpleCubeIdxs[] = {
		0, 1, 0, 3, 2, 1, 2, 3,
		4, 5, 4, 7, 6, 5, 6, 7,
		0, 4, 1, 5, 2, 6, 3, 7 };
		*/
		uint16_t l_SimpleCubeIdxs[] = {
			0, 2, 1, 3, 2, 0, // bot
			0, 4, 3, 3, 4, 7, // front
			0, 1, 4, 4, 1, 5,
			3, 7, 2, 2, 7, 6,
			1, 2, 5, 2, 6, 5,
			4, 5, 6, 6, 7, 4,
		};

		m_SimpleCube = new CTriangleListRenderableIndexed16Vertexs<MV_POSITION_COLOR_VERTEX>(l_SimpleCubeVtxs, 8, l_SimpleCubeIdxs, 36, false);
		m_SimpleCubeBSRadi = 1.224745f;
		m_SimpleCubeBBMin = Vect3f(-.5f, 0.f, -.5f);
		m_SimpleCubeBBMax = Vect3f(.5f, 1.f, .5f);
	}
}

void CDebugRender::InitializeDebugLights()
{
	CLightManager* l_LightManager = CEngine::GetSingleton().GetLightManager();
	size_t l_Size=l_LightManager->GetResourcesVector().size();
	
	for(size_t i=0;i<l_Size;++i)
	{
		CLight *l_Light = l_LightManager->GetResourceById(i);

		if(l_Light!=NULL)
		{
			CRenderableVertexs* m_DebugLight;

			Vect3f l_Pos = l_Light->GetPosition();
			CColor l_Color = l_Light->GetColor();

			if(l_Light->GetType() == CLight::DIRECTIONAL)
			{
				Vect3f l_Direction = ((CDirectionalLight*)l_Light)->GetDirection();
				l_Direction.Normalize(1.0f);

				MV_POSITION_COLOR_VERTEX l_AxisVtxs[8] = //6
				{
					{ Vect3f(l_Pos.x, l_Pos.y, l_Pos.z), CColor(0.0f,0.0f,0.0f,1.0f) },
					{ Vect3f(l_Pos.x+1.0f, l_Pos.y, l_Pos.z), CColor(0.0f,0.0f,0.0f,1.0f) },

					{ Vect3f(l_Pos.x, l_Pos.y, l_Pos.z), CColor(0.0f,0.0f,0.0f,1.0f) },
					{ Vect3f(l_Pos.x, l_Pos.y+1.0f, l_Pos.z), CColor(0.0f,0.0f,0.0f,1.0f)},

					{ Vect3f(l_Pos.x, l_Pos.y, l_Pos.z), CColor(0.0f,0.0f,0.0f,1.0f)},
					{ Vect3f(l_Pos.x, l_Pos.y, l_Pos.z+1.0f), CColor(0.0f,0.0f,0.0f,1.0f)},

					{ Vect3f(l_Pos.x, l_Pos.y, l_Pos.z), CColor(1.0f,1.0f,1.0f,1.0f)},
					{ Vect3f(l_Pos.x+(l_Direction.x*2.0f), l_Pos.y+(l_Direction.y*2.0f), l_Pos.z+(l_Direction.z*2.0f)), l_Color}
					
				};

				m_DebugLight = new CLinesListRenderableVertexs<MV_POSITION_COLOR_VERTEX>(l_AxisVtxs, 8, 4, false); //6,3
				m_DebugLights.push_back(m_DebugLight);
			}
			else if(l_Light->GetType() == CLight::OMNI)
			{
				MV_POSITION_COLOR_VERTEX l_SimpleCubeVtxs[] =
				{
					{ Vect3f(l_Pos.x+0.25f, l_Pos.y+0.0f, l_Pos.z+0.25f), l_Color },   // 0
					{ Vect3f(l_Pos.x+0.25f, l_Pos.y+0.0f, l_Pos.z-0.25f), l_Color },  // 1
					{ Vect3f(l_Pos.x-0.25f, l_Pos.y+0.0f, l_Pos.z-0.25f), l_Color }, // 2
					{ Vect3f(l_Pos.x-0.25f, l_Pos.y+0.0f, l_Pos.z+0.25f), l_Color },  // 3
					{ Vect3f(l_Pos.x+0.25f, l_Pos.y+0.5f, l_Pos.z+0.25f), l_Color },   // 4
					{ Vect3f(l_Pos.x+0.25f, l_Pos.y+0.5f, l_Pos.z-0.25f), l_Color },  // 5
					{ Vect3f(l_Pos.x-0.25f, l_Pos.y+0.5f, l_Pos.z-0.25f), l_Color },   // 6
					{ Vect3f(l_Pos.x-0.25f, l_Pos.y+0.5f, l_Pos.z+0.25f), l_Color },  // 7
				};

				uint16_t l_SimpleCubeIdxs[] = {
					0, 2, 1, 3, 2, 0, // bot
					0, 4, 3, 3, 4, 7, // front
					0, 1, 4, 4, 1, 5,
					3, 7, 2, 2, 7, 6,
					1, 2, 5, 2, 6, 5,
					4, 5, 6, 6, 7, 4,
				};

				m_DebugLight = new CTriangleListRenderableIndexed16Vertexs<MV_POSITION_COLOR_VERTEX>(l_SimpleCubeVtxs, 8, l_SimpleCubeIdxs, 36, false);
				m_DebugIndexedLights.push_back(m_DebugLight);
			}
			else if(l_Light->GetType() == CLight::SPOT)
			{
				Vect3f l_Direction = ((CSpotLight*)l_Light)->GetDirection();
				l_Direction.Normalize(1.0f);

				MV_POSITION_COLOR_VERTEX l_SimpleTriangleVtxs[3] =
				{
					{ Vect3f(l_Pos.x-0.5f, l_Pos.y-0.5f, l_Pos.z+0.5f), CColor(0.0f,0.0f,0.0f,1.0f) },
					{ Vect3f(l_Pos.x+(l_Direction.x*1.5f), l_Pos.y+(l_Direction.y*1.5f), l_Pos.z+(l_Direction.z*1.5f)), l_Color  }, //{ Vect3f(l_Pos.x+0.0f, l_Pos.y+0.5f, l_Pos.z+0.5f), CColor(0.0f,0.0f,0.0f,1.0f)  },
					{ Vect3f(l_Pos.x+0.5f, l_Pos.y-0.5f, l_Pos.z+0.5f), CColor(0.0f,0.0f,0.0f,1.0f)  },
				};
				m_DebugLight = new CTrianglesListRenderableVertexs<MV_POSITION_COLOR_VERTEX>(l_SimpleTriangleVtxs, 3, 1, false);
				m_DebugLights.push_back(m_DebugLight);
			}
		}
	}
}

void CDebugRender::InitializeASTarDebug(std::vector<Vect3f> WayPoints)
{
	CColor l_Color = CColor(1.0f,0.0f,0.0f,1.0f);

	for(size_t i=0;i<WayPoints.size();++i)
	{
		CRenderableVertexs* m_DebugPoint;

		MV_POSITION_COLOR_VERTEX l_SimpleCubeVtxs[] =
		{
			{ Vect3f(WayPoints[i].x, WayPoints[i].y, WayPoints[i].z), l_Color },   // 0
			{ Vect3f(WayPoints[i].x, WayPoints[i].y, WayPoints[i].z), l_Color },  // 1
			{ Vect3f(WayPoints[i].x, WayPoints[i].y, WayPoints[i].z), l_Color }, // 2
			{ Vect3f(WayPoints[i].x, WayPoints[i].y, WayPoints[i].z), l_Color },  // 3
			{ Vect3f(WayPoints[i].x, WayPoints[i].y, WayPoints[i].z), l_Color },   // 4
			{ Vect3f(WayPoints[i].x, WayPoints[i].y, WayPoints[i].z), l_Color },  // 5
			{ Vect3f(WayPoints[i].x, WayPoints[i].y, WayPoints[i].z), l_Color },   // 6
			{ Vect3f(WayPoints[i].x, WayPoints[i].y, WayPoints[i].z), l_Color },  // 7
		};

		uint16_t l_SimpleCubeIdxs[] = {
			0, 2, 1, 3, 2, 0, // bot
			0, 4, 3, 3, 4, 7, // front
			0, 1, 4, 4, 1, 5,
			3, 7, 2, 2, 7, 6,
			1, 2, 5, 2, 6, 5,
			4, 5, 6, 6, 7, 4,
		};

		m_DebugPoint = new CTriangleListRenderableIndexed16Vertexs<MV_POSITION_COLOR_VERTEX>(l_SimpleCubeVtxs, 8, l_SimpleCubeIdxs, 36, false);

		m_DebugWayPoints.push_back(m_DebugPoint);
	}
}


CDebugRender::~CDebugRender()
{
	CHECKED_DELETE(m_SimpleTriangle);
	CHECKED_DELETE(m_ClassicBlendTriangle);
	CHECKED_DELETE(m_PremultBlendTriangle);
	CHECKED_DELETE(m_SimpleGrid);
	CHECKED_DELETE(m_SimpleCube);
	CHECKED_DELETE(m_Axis);
	CHECKED_DELETE(m_LongAxis);

	for(size_t i=0;i<m_DebugLights.size();++i)
	{
		CHECKED_DELETE(m_DebugLights[i]);
	}

	for(size_t i=0;i<m_DebugIndexedLights.size();++i)
	{
		CHECKED_DELETE(m_DebugIndexedLights[i]);
	}

	for(size_t i=0;i<m_DebugWayPoints.size();++i)
	{
		CHECKED_DELETE(m_DebugWayPoints[i]);
	}

	m_DebugWayPoints.clear();
	m_DebugLights.clear();
	m_DebugIndexedLights.clear();
}

void CDebugRender::RenderDebugLights(CContextManager* ContextManager)
{
	for(size_t i=0;i<m_DebugLights.size();++i)
	{
		Mat44f world;
		world.SetIdentity();
		ContextManager->SetWorldMatrix(world);
		ContextManager->Draw(m_DebugLights[i]);
	}

	for(size_t i=0;i<m_DebugIndexedLights.size();++i)
	{
		Mat44f world;
		world.SetIdentity();
		ContextManager->SetWorldMatrix(world);
		ContextManager->DrawIndexed(m_DebugIndexedLights[i]);
	}
}

void CDebugRender::RenderDebugLayer(CContextManager* ContextManager, CRenderableObjectsManager* Layer)
{
	size_t l_Size = Layer->GetResourcesVector().size();
	
	for(size_t i=0;i<l_Size;++i)
	{
		Mat44f world;
		world.SetIdentity();
		ContextManager->SetWorldMatrix(Layer->GetResourceById(i)->GetTransform());
		ContextManager->Draw(GetAxis());

		/*CRenderableObject* l_RObject = Layer->GetResourceById(i);
		MV_POSITION_COLOR_VERTEX l_AxisVtxs[6] =
		{
			{ Vect3f(0.0f, 0.0f, 0.0f), CColor(1.0f, 0.0f, 0.0f, 1.0f) },
			{ l_RObject->GetRight(), CColor(1.0f, 0.0f, 0.0f, 1.0f) },

			{ Vect3f(0.0f, 0.0f, 0.0f), CColor(0.0f, 1.0f, 0.0f, 1.0f) },
			{ l_RObject->GetUp(), CColor(0.0f, 1.0f, 0.0f, 1.0f) },

			{ Vect3f(0.0f, 0.0f, 0.0f), CColor(0.0f, 0.0f, 1.0f, 1.0f) },
			{ l_RObject->GetForward(), CColor(0.0f, 0.0f, 1.0f, 1.0f) }
		};
		
		m_Prueba = CLinesListRenderableVertexs<MV_POSITION_COLOR_VERTEX>(l_AxisVtxs, 6, 3, false);
		ContextManager->Draw(&m_Prueba);*/
	}
}

void CDebugRender::RenderDebugGrid(CContextManager* ContextManager)
{
	Mat44f world;
	world.SetIdentity();
	ContextManager->SetWorldMatrix(world);
	ContextManager->DrawIndexed(GetSimpleGrid());
}

void CDebugRender::RenderDebugAStar(CContextManager* ContextManager)
{
	for(size_t i=0;i<m_DebugWayPoints.size();++i)
	{
		Mat44f world;
		world.SetIdentity();
		ContextManager->SetWorldMatrix(world);
		ContextManager->DrawIndexed(m_DebugWayPoints[i]);
	}
}

/*
	world.SetIdentity();
	m_ContextManager->SetWorldMatrix(world);
	m_ContextManager->DrawIndexed(m_DebugRender->¡GetAxis());
	
	world.SetIdentity();
	world.SetFromPos(10, 0, 0);
	m_ContextManager->SetWorldMatrix(world);
	m_ContextManager->Draw(m_DebugRender->GetClassicBlendTriangle(), CContextManager::RS_SOLID, CContextManager::DSS_OFF, CContextManager::BLEND_CLASSIC);
	
	world.SetIdentity();
	world.SetFromPos(0, 0, -10);
	m_ContextManager->SetWorldMatrix(world);
	m_ContextManager->Draw(m_DebugRender->GetPremultBlendTriangle(), CContextManager::RS_SOLID, CContextManager::DSS_OFF, CContextManager::BLEND_PREMULT);
	*/
