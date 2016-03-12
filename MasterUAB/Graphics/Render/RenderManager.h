#ifndef _RENDERMANAGER_H
#define _RENDERMANAGER_H

#include "Camera\Camera.h"
#include "Render\Frustum.h"
#include "RenderableObjects\RenderableObject.h"
#include <vector>
#include "Render\ContextManager.h"

class CRenderableObjectsManager;
class CTexture;
class CDebugRender;
class CRenderableVertexs;;

class CRenderManager
{
public:
	CRenderManager(CContextManager *ContextManager);
	~CRenderManager();

	void InitializeDebugRender();

	void SetCurrentCamera(const CCamera& _CurrentCamera);
	void SetDebugCamera(const CCamera& _DebugCamera) { m_DebugCamera = _DebugCamera; }

	void SetUseDebugCamera(bool _use) { m_UseDebugCamera = _use; }

	bool AddRenderableObjectToRenderList(const CRenderableObject* _RenderableObject);

	void Render(CContextManager* _Context);

	CContextManager * GetContextManager() const {return m_ContextManager;}

	void DrawScreenQuad(CEffectTechnique *EffectTechnique, CTexture *Texture, float x, float y, float Width, float Height, const CColor &Color);

	void RenderDebugLights();
	void RenderDebugLayer(CRenderableObjectsManager* Layer);
	void RenderDebugGrid();
	void RenderDebugAStar();

	CCamera GetCurrentCamera() const {return m_CurrentCamera;};
	CDebugRender* GetDebugRender() const {return m_DebugRender;};

private:
	CContextManager *m_ContextManager;
	CDebugRender *m_DebugRender;

	CCamera m_CurrentCamera;
	CFrustum m_CullFrustum;
	CCamera m_DebugCamera;
	bool m_UseDebugCamera;

	CRenderableVertexs *m_DrawQuadRV;

	size_t m_CurrentRenderlistLength;
	std::vector<const CRenderableObject*> m_RenderableObjects;

};

#endif
