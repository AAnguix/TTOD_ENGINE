#ifndef _GAME_H_
#define _GAME_H_

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

class CGraphicsManager;
class CEngineSettings;

class CGame
{
public:
	CGame();
	CGame(const CGame&);
	~CGame();

	bool Initialize();
	void Shutdown();
	void Run() const;
	void SwitchCamera();
	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	bool Frame() const;
	bool Update(float ElapsedTime) const;
	void InitializeWindows(int&, int&, bool&, bool&, bool&);
	void ShutdownWindows();

private:
	LPCSTR m_ApplicationName;
	HINSTANCE m_Hinstance;
	HWND m_Hwnd;

	CGraphicsManager* m_Graphics;
	CEngineSettings* m_EngineSettings;
	int m_CurrentCamera;

};

static LRESULT CALLBACK MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

static CGame* ApplicationHandle = nullptr;


#endif