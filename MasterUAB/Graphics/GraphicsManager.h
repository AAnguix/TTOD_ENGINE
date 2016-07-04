#ifndef _GRAPHICSMANAGER_H
#define _GRAPHICSMANAGER_H

#include "Render\ContextManager.h"
class CRenderManager;

class CGraphicsManager
{
public:
	CGraphicsManager();
	CGraphicsManager(const CGraphicsManager&);
	~CGraphicsManager();

	bool Initialize(int ScreenWidth, int ScreenHeight, HWND, bool FullScreen, bool VSync, bool D3DDebug);
	void Shutdown();
	bool Frame();

private:
	bool Render();

private:
	CContextManager* m_ContextManager;
	CRenderManager* m_RenderManager;
};

#endif