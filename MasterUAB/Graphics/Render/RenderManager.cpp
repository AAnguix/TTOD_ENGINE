#include "Render\RenderManager.h"
#include "Render\DebugRender.h"
#include "Materials\MaterialManager.h"
#include "Engine.h"
#include "RenderableObjects\RenderableObjectsManager.h"
#include "RenderableObjects\MeshInstance.h"
#include "Textures\Texture.h"
#include "Vertex\VertexTypes.h"
#include "Vertex\RenderableVertexs.h"
#include "Lights\LightManager.h"
#include "Lights\SpotLight.h"

CRenderManager::CRenderManager(CContextManager *ContextManager)
: m_ContextManager(ContextManager)
, m_UseDebugCamera(false)
, m_CurrentRenderlistLength(0)
, m_DebugRender(NULL)
{
	
}

CRenderManager::~CRenderManager()
{
	CHECKED_DELETE(m_DebugRender);
	CHECKED_DELETE(m_DrawQuadRV);
}

void CRenderManager::InitializeDebugRender()
{
	m_DebugRender = new CDebugRender;
	m_DebugRender->InitializeDebugLights();
	
	MV_POSITION4_COLOR_TEXTURE_VERTEX l_ScreenVertexsQuad[4]=
	{
		{Vect4f(-1.0f, 1.0f, 0.5f, 1.0f), CColor(1.0f, 1.0f, 1.0f, 1.0f), Vect2f(0.0f, 0.0f)},
		{Vect4f(-1.0f, -1.0f, 0.5f, 1.0f), CColor(1.0f, 1.0f, 1.0f, 1.0f), Vect2f(0.0f, 1.0f)},
		{Vect4f(1.0f, 1.0f, 0.5f, 1.0f), CColor(1.0f, 1.0f, 1.0f, 1.0f), Vect2f(1.0f, 0.0f)},
		{Vect4f(1.0f, -1.0f, 0.5f, 1.0f), CColor(1.0f, 1.0f, 1.0f, 1.0f), Vect2f(1.0f, 1.0f)}
	};
	
	m_DrawQuadRV = new CTrianglesStripRenderableVertexs<MV_POSITION4_COLOR_TEXTURE_VERTEX>(l_ScreenVertexsQuad, 4, 2, false);
}

void CRenderManager::SetCurrentCamera(const CCamera& _CurrentCamera)
{
	m_CurrentCamera = _CurrentCamera;
	Mat44f viewProj = m_CurrentCamera.GetView() * m_CurrentCamera.GetProjection();
	m_CullFrustum.Update(viewProj);
}

bool CRenderManager::AddRenderableObjectToRenderList(const CRenderableObject* _RenderableObject)
{
	// Alguien se atrebe a arreglar el frustum?

	//if (m_CullFrustum.SphereVisible(_RenderableObject->GetTransform().Position, _RenderableObject->GetBoundingRadius()))
	{
		if (m_CurrentRenderlistLength == m_RenderableObjects.size())
		{
			m_RenderableObjects.push_back(_RenderableObject);
		}
		else
		{
			m_RenderableObjects[m_CurrentRenderlistLength];
		}
		++m_CurrentRenderlistLength;
		return true;
	}
	//else
	//{
	//	return false;
	//}
}

void CRenderManager::Render(CContextManager* _Context/*, CMaterialManager* _MaterialManager*/)
{
	if (m_UseDebugCamera)
	{
		_Context->SetCamera(m_DebugCamera);
	}
	else
	{
		_Context->SetCamera(m_CurrentCamera);
	}
	/*AddRenderableObjectToRenderList(CEngine::GetSingleton().GetRenderableObjectsManager()->GetResource("Box002"));
	for (size_t i = 0; i < m_CurrentRenderlistLength; ++i)
	{
		//const CRenderableObject* l_RenderableObject = m_RenderableObjects[i];

		//((CMeshInstance*)l_RenderableObject)->Render(CEngine::GetSingleton().GetRenderManager());
	}*/

	//CEngine::GetSingleton().GetRenderableObjectsManager()->Render(this);

	// TODO crear un vector para objetos transparentes
	//struct BlendedSubmesh
	//{
	//    const CRenderableVertexs* vertices;
	//    Mat44f world;
	//    const CMaterial* material;
	//    const Vect3f& position;
	//};
	//std::vector<BlendedSubmesh> l_SubmeshesWithBlend;

	/*for (size_t i = 0; i < m_CurrentRenderlistLength; ++i)
	{
		const CRenderableObject* l_RenderableObject = m_RenderableObjects[i];
		const SPositionOrientation& l_Transform = l_RenderableObject->GetTransform();

		Mat44f world;
		world.SetFromPosAndAnglesYXZ(l_Transform.Position, l_Transform.Yaw, l_Transform.Pitch, l_Transform.Roll);
		_Context->SetWorldMatrix(world);

		for (int j = 0; j < l_RenderableObject->GetNumSubmeshes(); ++j)
		{
			const CRenderableObject::SSubmesh& l_Submesh = l_RenderableObject->GetSubmesh(j);
			const CMaterial* l_Material = _MaterialManager->GetMaterial(l_Submesh.material);
			// TODO no pintar el objeto, sino añadirlo a la lista l_SubmeshesWithBlend si tiene blend
			// l_Material->HasBlending();

			l_Material->SetShaderParameters(_Context);
			_Context->Draw(l_Submesh.vertices, l_Material->GetRasterizerState(), l_Material->GetDepthStencilState(), l_Material->GetBlendState());
		}
	}*/

	// TODO: Ordenar objetos según la distáncia a la cámara
	// NOTA: El quicksort es más rápido que el buble sort cuando la lista tiene más de ~100 objetos. NO OS MATÉIS SI NO HACE FALTA.
	//const Vect3f& l_CameraPosition = m_CurrentCamera.GetPosition();
	//for (int i = 0; i < l_SubmeshesWithBlend.size(); ++i)
	//{
	//    float l_DistanceSQ = l_SubmeshesWithBlend[i].position.SquaredLength(l_CameraPosition);
	//}

	// TODO: Pintar objetos translúcidos
	//for (int i = 0; i < l_SubmeshesWithBlend.size(); ++i)
	//{
	//
	//    // TODO render
	//
	//}

	m_CurrentRenderlistLength = 0;
}

CFrustum CRenderManager::GetFrustum() const
{
	return m_CullFrustum;
}

void CRenderManager::DrawScreenQuad(CEffectTechnique *EffectTechnique, CTexture *Texture, float x, float y, float Width, float Height, const CColor &Color)
{
	CEffectManager::m_SceneEffectParameters.m_BaseColor=Color;
	if(Texture!=NULL)
	Texture->Activate(0);
	D3D11_VIEWPORT l_Viewport;
	l_Viewport.Width=Width*m_ContextManager->GetViewPort().Width;
	l_Viewport.Height=Height*m_ContextManager->GetViewPort().Height;
	l_Viewport.MinDepth = 0.0f;
	l_Viewport.MaxDepth = 1.0f;
	l_Viewport.TopLeftX = x*m_ContextManager->GetViewPort().Width;
	l_Viewport.TopLeftY = y*m_ContextManager->GetViewPort().Height;
	m_ContextManager->GetDeviceContext()->RSSetViewports(1, &l_Viewport);
	m_DrawQuadRV->Render(this, EffectTechnique,&CEffectManager::m_SceneEffectParameters);
	m_ContextManager->GetDeviceContext()->RSSetViewports(1, &m_ContextManager->GetViewPort());
}

void CRenderManager::RenderDebugLights()
{
	m_DebugRender->RenderDebugLights(m_ContextManager);
	//m_DebugRender->RenderDebugAStar(m_ContextManager);
}

void CRenderManager::RenderDebugLayer(CRenderableObjectsManager* Layer)
{
	m_DebugRender->RenderDebugLayer(m_ContextManager, Layer);
}

void CRenderManager::RenderDebugGrid()
{
	m_DebugRender->RenderDebugGrid(m_ContextManager);
}

void CRenderManager::RenderDebugAStar()
{
	m_DebugRender->RenderDebugAStar(m_ContextManager);
}
