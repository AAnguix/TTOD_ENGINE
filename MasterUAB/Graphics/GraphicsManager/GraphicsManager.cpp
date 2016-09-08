#include "GraphicsManager.h"
#include "Engine\Engine.h"
#include "SceneRendererCommands\SceneRendererCommandManager.h"
#include "Render\RenderManager.h"
#include "Render\ContextManager.h"

CGraphicsManager::CGraphicsManager()
:m_ContextManager(nullptr)
,m_RenderManager(nullptr)
{
	
}

CGraphicsManager::~CGraphicsManager()
{
}


bool CGraphicsManager::Initialize(int ScreenWidth, int ScreenHeight, HWND Hwnd, bool FullScreen, bool VSync, bool D3DDebug)
{
	bool l_Result;

	m_ContextManager = new CContextManager();
	if (!m_ContextManager){	return false; }

	m_RenderManager = new CRenderManager(m_ContextManager);
	CEngine::GetSingleton().SetRenderManager(m_RenderManager);

	ShowWindow(Hwnd, SW_SHOWDEFAULT);

	//m_RenderManager->InitializeDebugRender();

	l_Result = m_ContextManager->Initialize(Hwnd, ScreenWidth, ScreenHeight, FullScreen, VSync, D3DDebug);
	if (!l_Result){return false;}

	//if (!FullScreen)
	//{
	l_Result = m_ContextManager->CreateBackBuffer(Hwnd, ScreenWidth, ScreenHeight);
	if (!l_Result){ false; }
	//}
	l_Result = m_ContextManager->InitStates();  
	if (!l_Result){ return false; }

	return true;
}


void CGraphicsManager::Shutdown()
{
	if (m_ContextManager)
	{
		m_ContextManager->Shutdown();
		delete m_ContextManager;
		m_ContextManager = 0;
	}
	if (m_RenderManager)
	{
		m_RenderManager->Shutdown();
		delete m_RenderManager;
		m_RenderManager = 0;
	}
}


bool CGraphicsManager::Frame() const
{
	return Render();
}

bool CGraphicsManager::Render() const
{
	return CEngine::GetSingleton().GetSceneRendererCommandManager()->Execute(*m_RenderManager);
}