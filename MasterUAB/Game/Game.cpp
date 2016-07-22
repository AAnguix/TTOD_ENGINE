#include <Windowsx.h>
#include <AntTweakBar.h>
#include <vld.h> //Visual Leak Detector

#include "Game.h"
#include "Engine\EngineSettings.h"
#include "Render\GraphicsStats.h"
#include "Engine\Engine.h"
#include "Input\InputManagerImplementation.h"
#include "Input\KeyBoardInput.h"
#include "LuabindManager\LuabindManager.h"
#include "Log\Log.h"
#include "Render\RenderManager.h"
#include "Resources\ResourceManager.h"

#include "Resources\resource.h"
#include "libxml\parser.h"

#pragma comment(lib, "Winmm.lib")

static void __stdcall SwitchCameraCallback(void* _app)
{
	((CGame*)_app)->SwitchCamera();
}

CGame::CGame()
:m_Graphics(0) 
,m_CurrentCamera(0)
{
}

CGame::CGame(const CGame& other)
{
}

CGame::~CGame()
{
}

void CGame::SwitchCamera()
{
	++m_CurrentCamera;
	if (m_CurrentCamera > 1)
	{
		m_CurrentCamera = 0;
	}
}


bool CGame::Initialize()
{
	int screenWidth, screenHeight;
	bool result;

	xmlInitParser(); //libXML Thread safer
	
	screenWidth = 0;
	screenHeight = 0;
	bool l_FullScreen = false;
	bool l_VSync = false;
	bool D3DDebug = false;

	InitializeWindows(screenWidth, screenHeight, l_FullScreen, l_VSync, D3DDebug);
	
	/*m_Input = new InputClass;
	if (!m_Input)
	{
		return false;
	}*/

	//m_Input->Initialize();

	m_Graphics = new CGraphicsManager;
	if (!m_Graphics)
	{
		return false;
	}

	CEngine &l_Engine = CEngine::GetSingleton();

	result = m_Graphics->Initialize(screenWidth, screenHeight, m_hwnd, l_FullScreen, l_VSync, D3DDebug);
	if (!result)
	{
		LOG("Unable to initialize graphics module");
		return false;
	}
	ShowWindow(m_hwnd, SW_SHOWDEFAULT);

	l_Engine.GetInputManager()->Initialize(m_hwnd);
	l_Engine.GetInputManager()->LoadCommandsFromFile("./Data/input.xml");

	l_Engine.Initialize(&m_hinstance);
	l_Engine.GetRenderManager()->InitializeDebugRender();

	//CAStar l_AStar;
	//m_RenderManager.GetDebugRender()->InitializeASTarDebug(l_AStar.SearchPathA(Vect3f(0.0f,0.0f,0.0f),Vect3f(-10.0f,0.0f,5.0f)));

	l_Engine.LoadLevelsCommonData();


	/*CEngine::GetSingleton().GetSceneRendererCommandManager()->Load("./Data/scene_renderer_commands.xml");*/
	l_Engine.GetLuabindManager()->RunLuaMain();

	CEngine::GetSingleton().GetLogManager()->Log("Game initialized properly.");

	return true;
}


void CGame::Shutdown()
{
	if (m_Graphics)
	{
		m_Graphics->Shutdown();
		delete m_Graphics;
		m_Graphics = 0;
	}

	/*if (m_Input)
	{
		delete m_Input;
		m_Input = 0;
	}*/

	ShutdownWindows();

	if (m_EngineSettings)
	{
		//m_EngineSettings->Shutdown();
		delete m_EngineSettings;
		m_EngineSettings = 0;
	}

	delete CEngine::GetSingletonPtr();

	return;
}

void CGame::Run()
{
	SwitchCamera();

	UpdateWindow(m_hwnd);

	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));
	
	bool l_Exit, result;

	bool hasFocus = true;

	__int64 cntsPerSec = 0;
	QueryPerformanceFrequency((LARGE_INTEGER*)&cntsPerSec);
	float secsPerCnt = 1.0f / (float)cntsPerSec;

	__int64 prevTimeStamp = 0;
	QueryPerformanceCounter((LARGE_INTEGER*)&prevTimeStamp);

	l_Exit = false;
	while (!l_Exit)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		if (msg.message == WM_QUIT)
		{
			l_Exit = true;
		}
		else
		{
			__int64 currTimeStamp = 0;
			QueryPerformanceCounter((LARGE_INTEGER*)&currTimeStamp);
			float l_ElapsedTime = (currTimeStamp - prevTimeStamp)*secsPerCnt;
			
			CEngine::GetSingleton().GetInputManager()->BeginFrame();
			result = Update(l_ElapsedTime);
			result = Frame();
			CEngine::GetSingleton().GetInputManager()->EndFrame();

			prevTimeStamp = currTimeStamp;
			if (!result)
			{
				l_Exit = true;
			}
		}

	}

	return;
}


bool CGame::Frame()
{
	bool result;

	/*if (m_Input->IsKeyDown(VK_ESCAPE))
	{
		return false;
	}*/

	result = m_Graphics->Frame();
	if (!result)
	{
		return false;
	}

	return true;
}

bool CGame::Update(float ElapsedTime)
{
	if (!CEngine::GetSingleton().LoadingLevel())
		CEngine::GetSingleton().Update(ElapsedTime);
	return true;
}

LRESULT CALLBACK CGame::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch (umsg)
	{
		case WM_KEYDOWN:
		{
			/*m_Input->KeyDown((unsigned int)wparam);*/
			return 0;
		}

		case WM_KEYUP:
		{
			/*m_Input->KeyUp((unsigned int)wparam);*/
			return 0;
		}

		default:
		{
			return DefWindowProc(hwnd, umsg, wparam, lparam);
		}
	}
}

void ToggleFullscreen(HWND Window, WINDOWPLACEMENT &WindowPosition)
{
	// This follows Raymond Chen's prescription
	// for fullscreen toggling, see:
	// http://blogs.msdn.com/b/oldnewthing/archive/2010/04/12/9994016.aspx

	DWORD Style = GetWindowLongW(Window, GWL_STYLE);
	if (Style & WS_OVERLAPPEDWINDOW)
	{
		MONITORINFO MonitorInfo = { sizeof(MonitorInfo) };
		if (GetWindowPlacement(Window, &WindowPosition) && GetMonitorInfoW(MonitorFromWindow(Window, MONITOR_DEFAULTTOPRIMARY), &MonitorInfo))
		{
			SetWindowLongW(Window, GWL_STYLE, Style & ~WS_OVERLAPPEDWINDOW);
			SetWindowPos(Window, HWND_TOP,
				MonitorInfo.rcMonitor.left, MonitorInfo.rcMonitor.top,
				MonitorInfo.rcMonitor.right - MonitorInfo.rcMonitor.left,
				MonitorInfo.rcMonitor.bottom - MonitorInfo.rcMonitor.top,
				SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
		}
	}
	else
	{
		SetWindowLongW(Window, GWL_STYLE, Style | WS_OVERLAPPEDWINDOW);
		SetWindowPlacement(Window, &WindowPosition);
		SetWindowPos(Window, 0, 0, 0, 0, 0,
			SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
	}
}

LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	TwEventWin(hWnd, msg, wParam, lParam);

	switch (msg)
	{
		case WM_SIZE:
		{
			CContextManager* l_ContextManager = CEngine::GetSingleton().GetRenderManager()->GetContextManager();
			if (l_ContextManager)
			{
				//if (wParam != SIZE_MINIMIZED)
				{
					// TODO: Resetear el AntTeakBar
					TwWindowSize(0, 0);
					l_ContextManager->ResizeBuffers(hWnd, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam));
					TwWindowSize((UINT)LOWORD(lParam), (UINT)HIWORD(lParam));
				}
			}
			return 0;
		}
		case WM_CLOSE:
		{
			PostQuitMessage(0);
			return 0;
		}
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
		case WM_CHAR:
		{
			CEngine::GetSingleton().GetInputManager()->GetKeyBoard()->SetLastChar(wParam);
			break;
		}
		case WM_SETCURSOR:
		{
			CEngine::GetSingleton().GetResourceManager()->UpdateCursor();
			return TRUE;
		}
		case WM_MOUSEMOVE:
		{
			int xPosAbsolute = GET_X_LPARAM(lParam);
			int yPosAbsolute = GET_Y_LPARAM(lParam);

			CEngine::GetSingleton().GetInputManager()->UpdateCursor(xPosAbsolute, yPosAbsolute);
		}
		case WM_SETFOCUS:
			//hasFocus = true;
			CEngine::GetSingleton().GetInputManager()->SetFocus(true);
			break;
		case  WM_KILLFOCUS:
			//hasFocus = false;
			CEngine::GetSingleton().GetInputManager()->SetFocus(false);
			break;
		case WM_SYSKEYDOWN:
		case WM_SYSKEYUP:
		case WM_KEYDOWN:
		{
			//CEngine::GetSingleton().GetInputManager()->KeyEventReceived(wParam, lParam);
			//break;
		}
		case WM_KEYUP:
		{
			bool WasDown = ((lParam & (1 << 30)) != 0);
			bool IsDown = ((lParam & (1 << 31)) == 0);
			bool Alt = ((lParam & (1 << 29)) != 0);

			if (WasDown != IsDown)
			{
				if (IsDown)
				{
					bool consumed = false;
					switch (wParam)
					{
					case VK_RETURN:
						if (Alt)
						{
							/*WINDOWPLACEMENT windowPosition = { sizeof(WINDOWPLACEMENT) };
							GetWindowPlacement(hWnd, &windowPosition);

							ToggleFullscreen(hWnd, windowPosition);
							consumed = true;*/
						}
						break;
					case VK_F4:
						if (Alt)
						{
							PostQuitMessage(0);
							consumed = true;
						}
						break;
					}
					if (consumed)
					{
						break;
					}
				}
			}
			CEngine::GetSingleton().GetInputManager()->KeyEventReceived(wParam, lParam);
			break;
		}
		default:
		{
			return ApplicationHandle->MessageHandler(hWnd, msg, wParam, lParam);
		}

	}//end switch( msg )
}

void CGame::InitializeWindows(int& screenWidth, int& screenHeight,bool& FullScreen, bool& VSync, bool& D3DDebug)
{
	WNDCLASSEX wc;
	//DEVMODE dmScreenSettings;
	//int posX, posY;

	ApplicationHandle = this;
	m_hinstance = GetModuleHandle(NULL);
	m_ApplicationName = "The Tale Of Degann";

	ZeroMemory(&wc, sizeof(WNDCLASSEX));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_CLASSDC; //CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = MsgProc;
	wc.cbClsExtra = 0L;
	wc.cbWndExtra = 0L;
	wc.hInstance = m_hinstance;
	wc.hIcon = HICON(LoadImage(m_hinstance, MAKEINTRESOURCE(IDI_GAME_ICON), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE));
	wc.hIconSm = wc.hIcon;

	wc.hCursor = NULL; //HCURSOR(LoadImage(m_hinstance, MAKEINTRESOURCE(102), IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE));  //LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL; //(HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_ApplicationName;
	
	RegisterClassEx(&wc);

	m_EngineSettings = new CEngineSettings();
	m_EngineSettings->LoadSettings("Data/engine_settings.xml");

	Vect2i l_ScreenSize = m_EngineSettings->GetScreenSize();
	screenWidth = l_ScreenSize.x;
	screenHeight = l_ScreenSize.y;
	Vect2i l_ScreenPosition = m_EngineSettings->GetScreenPosition();
	Vect2i l_ScreenResolution = m_EngineSettings->GetScreenResolution();
	FullScreen = m_EngineSettings->GetFullScreen();
	VSync = m_EngineSettings->VSyncEnabled();

	RECT rc = { 0, 0, l_ScreenResolution.x, l_ScreenResolution.y };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

	/*screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	if (FULL_SCREEN)
	{
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		posX = posY = 0;
	}
	else
	{
		screenWidth = 800;
		screenHeight = 600;
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}*/

	m_hwnd = CreateWindow(m_ApplicationName, m_ApplicationName, WS_OVERLAPPEDWINDOW, 100, 100, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, wc.hInstance, NULL);
	
	//m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName,WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,posX, posY, screenWidth, screenHeight, NULL, NULL, m_hinstance, NULL);
	
	//SetForegroundWindow(m_hwnd);
	//SetFocus(m_hwnd);

	//ShowCursor(true);

	return;
}


void CGame::ShutdownWindows()
{
	ShowCursor(false);

	if (m_EngineSettings->GetFullScreen())
	{
		ChangeDisplaySettings(NULL, 0);
	}

	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	UnregisterClass(m_ApplicationName, m_hinstance);
	m_hinstance = NULL;

	// Release the pointer to this class.
	ApplicationHandle = NULL;

	return;
}


//LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
//{
//	switch (umessage)
//	{
//	case WM_DESTROY:
//	{
//		PostQuitMessage(0);
//		return 0;
//	}
//
//	case WM_CLOSE:
//	{
//		PostQuitMessage(0);
//		return 0;
//	}
//	default:
//	{
//		return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
//	}
//	}
//}