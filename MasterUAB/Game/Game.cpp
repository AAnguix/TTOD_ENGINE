#include <Windowsx.h>
#include <AntTweakBar.h>
#include <vld.h> //Visual Leak Detector

#include "Game.h"
#include "GraphicsManager\GraphicsManager.h"
#include "Engine\EngineSettings.h"
#include "Render\GraphicsStats.h"
#include "Engine\Engine.h"
#include "Input\InputMapperImplementation.h"

#include "Input\KeyBoardInput.h"
#include "LuabindManager\LuabindManager.h"
#include "Log\Log.h"
#include "Render\RenderManager.h"
#include "Resources\ResourceManager.h"

#include "Resources\resource.h"
#include "libxml\parser.h"

#pragma comment(lib, "Winmm.lib")

#include "Input\InputMapper.h"
#include <luabind/luabind.hpp>

//Forward dec.
void InputCallback(InputMapping::SMappedInput& inputs);
void InputGamepadCallback(InputMapping::SMappedInput& inputs);

int LastX = 0;
int LastY = 0;

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
	//ShowWindow(m_hwnd, SW_SHOWDEFAULT);

	l_Engine.SetEngineSettings(m_EngineSettings);
	l_Engine.GetInputMapper()->Initialize(L"./Data/InputContexts.txt",m_hwnd);
	l_Engine.GetInputMapper()->PushContext(L"maincontext");
	l_Engine.GetInputMapper()->AddCallback(InputCallback, 0);

	l_Engine.Initialize(&m_hinstance);
	l_Engine.GetRenderManager()->InitializeDebugRender();

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

	ShutdownWindows();

	if (m_EngineSettings)
	{
		delete m_EngineSettings;
		m_EngineSettings = 0;
	}

	delete CEngine::GetSingletonPtr();

	return;
}

void CGame::Run()
{
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
			
			CEngine::GetSingleton().GetInputMapper()->BeginFrame();
			CEngine::GetSingleton().GetInputMapper()->Dispatch();
			result = Update(l_ElapsedTime);
			result = Frame();
			CEngine::GetSingleton().GetInputMapper()->Clear();
			CEngine::GetSingleton().GetInputMapper()->EndFrame();

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
	bool l_Result = m_Graphics->Frame();
	if (!l_Result)
		return false;
	
	return true;
}

bool CGame::Update(float ElapsedTime)
{
	if (!CEngine::GetSingleton().LoadingLevel())
		CEngine::GetSingleton().Update(ElapsedTime);
	
	return true;
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


bool ConvertWParamToOSButton(WPARAM wparam, InputMapping::EOSInputButtonX& Button)
{
	switch (wparam)
	{
		case 13: Button = InputMapping::OS_INPUT_BUTTON_ENTER; break;
		case 27: Button = InputMapping::OS_INPUT_BUTTON_ESC;	break;
		case 32: Button = InputMapping::OS_INPUT_BUTTON_ESPACE;	break;
				 
		case 48: Button = InputMapping::OS_INPUT_BUTTON_ZERO;	break;
		case 49: Button = InputMapping::OS_INPUT_BUTTON_ONE;	break;
		case 50: Button = InputMapping::OS_INPUT_BUTTON_TWO;	break;
		case 51: Button = InputMapping::OS_INPUT_BUTTON_THREE;	break;
		case 52: Button = InputMapping::OS_INPUT_BUTTON_FOUR;	break;
		case 53: Button = InputMapping::OS_INPUT_BUTTON_FIVE;	break;
		case 54: Button = InputMapping::OS_INPUT_BUTTON_SIX;	break;
		case 55: Button = InputMapping::OS_INPUT_BUTTON_SEVEN;	break;
		case 56: Button = InputMapping::OS_INPUT_BUTTON_EIGHT;	break;
		case 57: Button = InputMapping::OS_INPUT_BUTTON_NINE;	break;
				 
		case 65: Button = InputMapping::OS_INPUT_BUTTON_A;	break;
		case 66: Button = InputMapping::OS_INPUT_BUTTON_B;	break;
		case 67: Button = InputMapping::OS_INPUT_BUTTON_C;	break;
		case 68: Button = InputMapping::OS_INPUT_BUTTON_D;	break;
		case 69: Button = InputMapping::OS_INPUT_BUTTON_E;	break;
		case 70: Button = InputMapping::OS_INPUT_BUTTON_F;	break;
		case 71: Button = InputMapping::OS_INPUT_BUTTON_G;	break;
		case 72: Button = InputMapping::OS_INPUT_BUTTON_H;	break;
		case 73: Button = InputMapping::OS_INPUT_BUTTON_I;	break;
		case 74: Button = InputMapping::OS_INPUT_BUTTON_J;	break;
		case 75: Button = InputMapping::OS_INPUT_BUTTON_K;	break;
		case 76: Button = InputMapping::OS_INPUT_BUTTON_L;	break;
		case 77: Button = InputMapping::OS_INPUT_BUTTON_M;	break;
		case 78: Button = InputMapping::OS_INPUT_BUTTON_N;	break;
		case 79: Button = InputMapping::OS_INPUT_BUTTON_O;	break;
		case 80: Button = InputMapping::OS_INPUT_BUTTON_P;	break;
		case 81: Button = InputMapping::OS_INPUT_BUTTON_Q;	break;
		case 82: Button = InputMapping::OS_INPUT_BUTTON_R;	break;
		case 83: Button = InputMapping::OS_INPUT_BUTTON_S;	break;
		case 84: Button = InputMapping::OS_INPUT_BUTTON_T;	break;
		case 85: Button = InputMapping::OS_INPUT_BUTTON_U;	break;
		case 86: Button = InputMapping::OS_INPUT_BUTTON_V;	break;
		case 87: Button = InputMapping::OS_INPUT_BUTTON_W;	break;
		case 88: Button = InputMapping::OS_INPUT_BUTTON_X;	break;
		case 89: Button = InputMapping::OS_INPUT_BUTTON_Y;	break;
		case 90: Button = InputMapping::OS_INPUT_BUTTON_Z;	break;
	
		case 112: Button = InputMapping::OS_INPUT_BUTTON_F1; break;
		case 113: Button = InputMapping::OS_INPUT_BUTTON_F2; break;
		case 114: Button = InputMapping::OS_INPUT_BUTTON_F3; break;
		case 115: Button = InputMapping::OS_INPUT_BUTTON_F4; break;
		case 116: Button = InputMapping::OS_INPUT_BUTTON_F5; break;
		case 117: Button = InputMapping::OS_INPUT_BUTTON_F6; break;
		case 118: Button = InputMapping::OS_INPUT_BUTTON_F7; break;
		case 119: Button = InputMapping::OS_INPUT_BUTTON_F8; break;
		case 120: Button = InputMapping::OS_INPUT_BUTTON_F9; break;
		default: return false; break;
	}

	return true;
}

void InputCallback(InputMapping::SMappedInput& inputs)
{
	/*AxisX = inputs.Ranges[InputMapping::RANGE_ONE];
	StateOne = inputs.States.find(InputMapping::STATE_ONE) != inputs.States.end();*/

	std::set<InputMapping::EAction>::iterator itAct;
	for (itAct = inputs.Actions.begin(); itAct != inputs.Actions.end(); ++itAct)
	{
		InputMapping::EAction l_Action = *itAct;
		luabind::call_function<void>(CEngine::GetSingleton().GetLuabindManager()->GetLuaState(), "InputActionCallback", l_Action);
	}

	std::set<InputMapping::EState>::iterator itState;
	for (itState = inputs.States.begin(); itState != inputs.States.end(); ++itState)
	{
		InputMapping::EState l_State = *itState;
		luabind::call_function<void>(CEngine::GetSingleton().GetLuabindManager()->GetLuaState(), "InputStateCallback", l_State);
	}

	if (CEngine::GetSingleton().Initialized())
		luabind::call_function<void>(CEngine::GetSingleton().GetLuabindManager()->GetLuaState(), "InputRangesCallback", inputs.Ranges[InputMapping::RANGE_ONE], inputs.Ranges[InputMapping::RANGE_TWO]);

	/*if (inputs.Actions.find(InputMapping::ACTION_ONE) != inputs.Actions.end())
		LOG("Action one raised");*/
}

void InputGamepadCallback(InputMapping::SMappedInput& inputs)
{
	std::set<InputMapping::EAction>::iterator itAct;
	for (itAct = inputs.Actions.begin(); itAct != inputs.Actions.end(); ++itAct)
	{
		//InputMapping::EAction l_Action = *itAct;
		//luabind::call_function<void>(CEngine::GetSingleton().GetLuabindManager()->GetLuaState(), "InputActionCallback", l_Action);
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
				if (SIZE_MINIMIZED != wParam)
				{
					// TODO: Resetear el AntTeakBar
					//TwWindowSize(0, 0);
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
			CEngine::GetSingleton().GetInputMapper()->GetKeyBoard()->SetLastChar(wParam);
			break;
		}
		case WM_SETCURSOR:
		{
			CEngine::GetSingleton().GetResourceManager()->UpdateCursor();
			return TRUE;
		}
		case WM_MOUSEMOVE:
		{
			int l_X = LOWORD(lParam);
			int l_Y = HIWORD(lParam);

			/*CEngine::GetSingleton().GetInputMapper()->SetOSAxisValue(InputMapping::OS_INPUT_AXIS_MOUSE_X, static_cast<double>(l_X - LastX));
			CEngine::GetSingleton().GetInputMapper()->SetOSAxisValue(InputMapping::OS_INPUT_AXIS_MOUSE_Y, static_cast<double>(l_Y - LastY));
			*/
			LastX = l_X;
			LastY = l_Y;

			/*int xPosAbsolute = GET_X_LPARAM(lParam);
			int yPosAbsolute = GET_Y_LPARAM(lParam);*/

			//NO DEBERIA NECESITAR ESTO, DIRECTX ACTUALIZA EL MOUSE
			//CEngine::GetSingleton().GetInputManager()->UpdateCursor(xPosAbsolute, yPosAbsolute);
		}
		case WM_SETFOCUS:
			//hasFocus = true;
			//CEngine::GetSingleton().GetInputManager()->SetFocus(true);
			CEngine::GetSingleton().GetInputMapper()->SetFocus(true);
			break;
		case  WM_KILLFOCUS:
			//hasFocus = false;
			//CEngine::GetSingleton().GetInputManager()->SetFocus(false);
			CEngine::GetSingleton().GetInputMapper()->SetFocus(false);
			break;
		case WM_SYSKEYDOWN:
		case WM_SYSKEYUP:
		case WM_KEYDOWN:
		{
			InputMapping::EOSInputButtonX l_OSButton;
			bool l_WasDown = ((lParam & (1 << 31)) != 0);

			bool l_Alt = ((lParam & (1 << 29)) != 0);
			bool l_Ctrl = false;
			switch (wParam)
			{
				case VK_CONTROL:
					l_Ctrl = true;
					break;
			}

			if (ConvertWParamToOSButton(wParam, l_OSButton))
			{
				InputMapping::SOSInputButtons l_OSButtons(l_OSButton, l_Alt, l_Ctrl);
				CEngine::GetSingleton().GetInputMapper()->SetOSButtonState(l_OSButtons, true, l_WasDown);
			}
			//CEngine::GetSingleton().GetInputManager()->KeyEventReceived(wParam, lParam);
			//break;
		}
		/*{
			Mapper.Dispatch();
			Mapper.Clear();
		}*/
		return 0;
		case WM_KEYUP:
		{
			bool WasDown = ((lParam & (1 << 30)) != 0);
			bool IsDown = ((lParam & (1 << 31)) == 0);

			bool l_Alt = ((lParam & (1 << 29)) != 0);
			bool l_Ctrl = false;
			switch (wParam)
			{
			case VK_CONTROL:
				l_Ctrl = true;
				break;
			}

			if (WasDown != IsDown)
			{
				if (IsDown)
				{
					bool consumed = false;
					switch (wParam)
					{
					case VK_RETURN:
						if (l_Alt)
						{
							/*WINDOWPLACEMENT windowPosition = { sizeof(WINDOWPLACEMENT) };
							GetWindowPlacement(hWnd, &windowPosition);

							ToggleFullscreen(hWnd, windowPosition);
							consumed = true;*/
						}
						break;
					case VK_F4:
						if (l_Alt)
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
			/*	CEngine::GetSingleton().GetInputManager()->KeyEventReceived(wParam, lParam);
			break;*/

			InputMapping::EOSInputButtonX l_OSButton;

			if (ConvertWParamToOSButton(wParam, l_OSButton))
			{
				InputMapping::SOSInputButtons l_OSButtons(l_OSButton, l_Alt, l_Ctrl);
				CEngine::GetSingleton().GetInputMapper()->SetOSButtonState(l_OSButtons, false, true);
			}
		}
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
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