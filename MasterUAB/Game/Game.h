#ifndef _GAME_H_
#define _GAME_H_

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "GraphicsManager.h"

class CEngineSettings;

class CGame
{
public:
	CGame();
	CGame(const CGame&);
	~CGame();

	bool Initialize();
	void Shutdown();
	void Run();
	void SwitchCamera();
	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	bool Frame();
	bool Update(float ElapsedTime);
	void InitializeWindows(int&, int&, bool&, bool&, bool&);
	void ShutdownWindows();

private:
	//LPCWSTR m_applicationName;
	LPCSTR m_ApplicationName;
	HINSTANCE m_hinstance;
	HWND m_hwnd;

	//InputClass* m_Input;
	CGraphicsManager* m_Graphics;
	CEngineSettings* m_EngineSettings;
	CCamera m_Camera;
	int m_CurrentCamera;

};

static LRESULT CALLBACK MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

static CGame* ApplicationHandle = 0;


#endif