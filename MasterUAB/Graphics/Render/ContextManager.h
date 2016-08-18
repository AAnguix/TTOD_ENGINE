#ifndef _CONTEXTMANAGER_H
#define _CONTEXTMANAGER_H

#include <d3d11.h>

#include "Effects\Effect.h"
#include "Camera\Camera.h"
#include "Effects\EffectParameters.h"
#include "Effects\EffectManager.h"
#include <assert.h>

class CRenderableVertexs;

class CContextManager
{
public:

	enum ERasterizedState
	{
		RS_WIREFRAME,
		RS_SOLID,
		RS_CULL_FRONT,
		RS_CULL_BACK,
		RS_COUNT
	};

	enum EDepthStencilState
	{
		DSS_DEPTH_ON,
		DSS_OFF,
		DSS_DEPTH_TEST,

		DSS_COUNT
	};

	enum EBlendState
	{
		BLEND_SOLID,
		BLEND_CLASSIC,
		BLEND_PREMULT,
		BLEND_DEFERRED_SHADING,

		BLEND_COUNT
	};

public:
	CContextManager();
	CContextManager(const CContextManager&);
	~CContextManager();

	bool ResizeBuffers(HWND hWnd, unsigned int Width, unsigned int Height);

	bool Initialize(HWND Hwnd, unsigned int ScreenWidth, unsigned int ScreenHeight, bool FullScreen, bool VSync, bool DebugMode);
	void SetFullScreen(bool Value, int ScreenWidth, int ScreenHeight);
	void Shutdown();
	
	bool CreateBackBuffer(HWND hWnd, unsigned int Width, unsigned int Height);
	bool InitStates();

	float GetAspectRatio() const { return (float)m_Width / (float)m_Height; }

	void BeginRender(CColor backgroundColor = CColor(.2f, .1f, .4f));
	void EndRender();
	void Draw(CRenderableVertexs* _VerticesToRender, ERasterizedState _RS = RS_SOLID, EDepthStencilState _DSS = DSS_DEPTH_ON, EBlendState _BS = BLEND_SOLID);
	void DrawIndexed(CRenderableVertexs* _VerticesToRender, ERasterizedState _RS = RS_SOLID, EDepthStencilState _DSS = DSS_DEPTH_ON, EBlendState _BS = BLEND_SOLID);

	ID3D11Device* GetDevice() const { return m_D3DDevice; }
	ID3D11DeviceContext* GetDeviceContext() const { return m_DeviceContext; }

	void SetBaseColor(const CColor& _Color) { assert(!"Not yet implemented");/*CEffectManager::m_SceneEffectParameters.m_BaseColor = _Color;*/ }
	void SetWorldMatrix(const Mat44f& _Model) { CEffectManager::m_SceneEffectParameters.m_World = _Model; }
	void SetCamera(const Mat44f& _View, const Mat44f& _Projection) { CEffectManager::m_SceneEffectParameters.m_View = _View; CEffectManager::m_SceneEffectParameters.m_Projection = _Projection; }
	void SetCamera(const CCamera& _Camera) { CEffectManager::m_SceneEffectParameters.m_View = _Camera.GetView(); CEffectManager::m_SceneEffectParameters.m_Projection = _Camera.GetProjection(); CEffectManager::m_SceneEffectParameters.m_CameraPosition = _Camera.GetPosition(); CEffectManager::m_SceneEffectParameters.m_CameraUpVector = _Camera.GetUp(); }
	//void SetCamera(const CCamera& _Camera) { CEffectManager::m_SceneEffectParameters.m_View = _Camera.GetView(); CEffectManager::m_SceneEffectParameters.m_Projection = _Camera.GetProjection(); }
	void SetDebugSize(float _Size) { assert(!"not yet implemented");/*CEffectManager::m_SceneEffectParameters.m_DebugRenderScale = _Size;*/ }

	//SceneRenderCommands
	void Clear(bool RenderTarget, bool DepthStencil);
	void Present();
	void SetMatrices(const CCamera& Camera);
	void SetTimeParameters(float ElapsedTime);
	D3D11_VIEWPORT GetViewPort();
	void SetDefaultViewPort();

	unsigned int GetFrameBufferWidth(){ return m_Width; };
	unsigned int GetFrameBufferHeight(){ return m_Height; };

	void SetRenderTargets(int NumViews, ID3D11RenderTargetView *const*RenderTargetViews, ID3D11DepthStencilView *DepthStencilView);
	
	void UnsetColorRenderTarget();
	void UnsetRenderTargets();

	void SetAlphaBlendState(ID3D11BlendState* AlphaBlendState);
	void EnableAlphaBlendState();
	void EnableAdditiveAlphaBlendState();
	void DisableAlphaBlendState();
	void EnableDeferredShadingBlendState();

	void SetRasterizerState(ERasterizedState RasterizerState);
	void SetDepthStencilState(EDepthStencilState DepthStencilState);

	IDXGISwapChain* GetSwapChain(){ return m_SwapChain; };

	ID3D11RenderTargetView * GetRenderTargetView() const { return m_RenderTargetView; }
	ID3D11DepthStencilView * GetDepthStencilView() const { return m_DepthStencilView; }

	bool SaveStencilBufferToFile();

	void GetVideoCardInfo(char*, int&);

private:

	bool InitRasterizerStates();
	bool InitDepthStencilStates();
	bool InitBlendingStates();

	DXGI_MODE_DESC GetDxgiModeDesc(unsigned int ScreenWidth, unsigned int ScreenHeight);

	DXGI_FORMAT GetDepthResourceFormat(DXGI_FORMAT DepthFormat);
	DXGI_FORMAT GetDepthShaderResourceViewFormat(DXGI_FORMAT Depthformat);

	ID3D11ShaderResourceView* m_StencilTexture;
	
	
	ID3D11Debug* m_D3DDebug;
	IDXGISwapChain* m_SwapChain;
	ID3D11Device* m_D3DDevice;
	ID3D11DeviceContext* m_DeviceContext;

	ID3D11RenderTargetView*	m_RenderTargetView;
	ID3D11Texture2D* m_DepthStencil;
	ID3D11DepthStencilView*	m_DepthStencilView;
	ID3D11RenderTargetView *const *m_CurrentRenderTargetViews;
	ID3D11DepthStencilView *m_CurrentDepthStencilView;

	unsigned int m_Width, m_Height;
	int m_NumViews;

	ID3D11RasterizerState*	m_RasterizerSates[RS_COUNT];
	ID3D11DepthStencilState * m_DepthStencilStates[DSS_COUNT];
	ID3D11BlendState* m_BlendStates[BLEND_COUNT];
	ID3D11BlendState* m_AlphaBlendState;
	ID3D11BlendState* m_AdditiveAlphaBlendState;
	D3D11_VIEWPORT m_Viewport;

	int m_VideoCardMemory;
	char m_VideoCardDescription[128];
	bool m_VSyncEnabled;

	unsigned int m_NumDisplayModes;
	DXGI_MODE_DESC* m_DisplayModeList;

	bool m_FullScreenEnabled;
};

#endif