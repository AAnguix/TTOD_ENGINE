#ifndef _GRAPHICSMANAGER_H
#define _GRAPHICSMANAGER_H

class CContextManager;
class CRenderManager;
#include <windows.h>

class CGraphicsManager
{
public:
	CGraphicsManager();
	~CGraphicsManager();

	bool Initialize(int ScreenWidth, int ScreenHeight, HWND, bool FullScreen, bool VSync, bool D3DDebug);
	void Shutdown();
	bool Frame() const;

private:
	bool Render() const;

private:
	CContextManager* m_ContextManager;
	CRenderManager* m_RenderManager;
};

#endif