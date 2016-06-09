#include <d3d11.h>
#include <D3DCommon.h>
#include <Windows.h>
#include <Windowsx.h>
#include "Engine.h"

#include <cassert>

// TODO: Activar AntTeakBar
#include <AntTweakBar.h>
#include "Render\GraphicsStats.h"

/*Base*/
#include "Math\Matrix44.h"
#include "Math\Vector4.h"


#include "Application.h"

/*Core*/
#include "Render\DebugRender.h"
#include "Input\InputManagerImplementation.h"
#include "DebugHelper\DebugHelperImplementation.h"
#include "Input\KeyBoardInput.h"

/*Graphics*/
#include "Render\ContextManager.h"
#include "Effects\Effect.h"
#include "Camera\Camera.h"
#include "Render\RenderManager.h"
#include "Render\ContextManager.h"
#include "RenderableObjects\RenderableObjectsManager.h"
#include "StaticMeshes\StaticMeshManager.h"

#pragma comment(lib, "Winmm.lib")

#define APPLICATION_NAME	"The Tale Of Degann"
#define APPLICATTION_WIDTH  1280 //1280
#define APPLICATTION_HEIGHT 720 //720

CContextManager s_Context;

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

//-----------------------------------------------------------------------------
// Name: MsgProc()
// Desc: The window's message handler
//-----------------------------------------------------------------------------
LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

	TwEventWin(hWnd, msg, wParam, lParam);
	//if (TwEventWin(hWnd, msg, wParam, lParam)) // send event message to AntTweakBar
//		return 0;

	switch (msg)
	{
		case WM_SIZE:
		{
			if (wParam != SIZE_MINIMIZED)
			{
				// TODO: Resetear el AntTeakBar
				 TwWindowSize(0, 0);
				s_Context.Resize(hWnd, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam));
				// TODO: Resetear el AntTeakBar
				TwWindowSize((UINT)LOWORD(lParam), (UINT)HIWORD(lParam));
			}
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
							WINDOWPLACEMENT windowPosition = { sizeof(WINDOWPLACEMENT) };
							GetWindowPlacement(hWnd, &windowPosition);

							ToggleFullscreen(hWnd, windowPosition);
							consumed = true;
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
			/*if (!hasFocus || !CEngine::GetSingleton().GetInputManager()->KeyEventReceived(msg.wParam, msg.lParam))
			{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			}*/
			break;
		}
		case WM_MOUSEMOVE:
			//if (hasFocus)
			{
				int xPosAbsolute = GET_X_LPARAM(lParam);
				int yPosAbsolute = GET_Y_LPARAM(lParam);

				CEngine::GetSingleton().GetInputManager()->UpdateCursor(xPosAbsolute, yPosAbsolute);
			}
			/*else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}*/
			break;
		/*default:
			TranslateMessage(&msg);
			DispatchMessage(&msg);*/

	}//end switch( msg )
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

void CheckMemoryLeaks()
{
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	//_CrtSetBreakAlloc(248261); //26598 29214 30923 32011
}

int APIENTRY WinMain(HINSTANCE _hInstance, HINSTANCE _hPrevInstance, LPSTR _lpCmdLine, int _nCmdShow)
{
	CheckMemoryLeaks();

	//Register the window class
	WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, APPLICATION_NAME, NULL };

	RegisterClassEx(&wc);

	RECT rc = {0, 0, APPLICATTION_WIDTH, APPLICATTION_HEIGHT};

	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

	//Create the application's window
	HWND hWnd = CreateWindow(APPLICATION_NAME, APPLICATION_NAME, WS_OVERLAPPEDWINDOW, 100, 100, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, wc.hInstance, NULL);

	s_Context.CreateContext(hWnd, APPLICATTION_WIDTH, APPLICATTION_HEIGHT);

	ShowWindow(hWnd, SW_SHOWDEFAULT);

	s_Context.CreateBackBuffer(hWnd, APPLICATTION_WIDTH, APPLICATTION_HEIGHT);
	s_Context.InitStates();

	{
		//Create and initialize application
		CApplication l_Application(&s_Context);
		CGraphicsStats l_CGraphicsStats;
		l_Application.Initialize(hWnd);
		l_Application.SwitchCamera();
		
		UpdateWindow(hWnd);

		MSG msg;
		ZeroMemory(&msg, sizeof(msg));

		// Añadir en el while la condición de salida del programa de la aplicación
		//DWORD m_PreviousTime = timeGetTime();
		//long int before = GetTickCount();

		bool hasFocus = true;

		__int64 cntsPerSec = 0;
		QueryPerformanceFrequency((LARGE_INTEGER*)&cntsPerSec);
		float secsPerCnt = 1.0f / (float)cntsPerSec;

		__int64 prevTimeStamp = 0;
		QueryPerformanceCounter((LARGE_INTEGER*)&prevTimeStamp);

		while (msg.message != WM_QUIT)
		{
			if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				__int64 currTimeStamp = 0;
				QueryPerformanceCounter((LARGE_INTEGER*)&currTimeStamp);
				float l_ElapsedTime = (currTimeStamp - prevTimeStamp)*secsPerCnt;

				CEngine::GetSingleton().GetInputManager()->BeginFrame();
					/*DWORD l_CurrentTime = timeGetTime();
					float m_ElapsedTime = (float)(l_CurrentTime - m_PreviousTime)*0.001f;
					m_PreviousTime = l_CurrentTime;*/

					l_Application.Update(l_ElapsedTime);
					l_Application.Render();

				CEngine::GetSingleton().GetInputManager()->EndFrame();

				prevTimeStamp = currTimeStamp;
			}
		}

		UnregisterClass(APPLICATION_NAME, wc.hInstance);
	}
	delete CEngine::GetSingletonPtr();
	
	//Release application memory
	s_Context.Dispose();

	return 0;
}
