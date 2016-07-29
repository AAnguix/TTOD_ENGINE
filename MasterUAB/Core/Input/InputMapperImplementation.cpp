//#include "pch.h"

#include "Input\InputMapperImplementation.h"
#include "Input\InputContext.h"
#include "Input\FileReader.h"
#include <fstream>
#include "Engine\Engine.h"
#include "Log\Log.h"

#include "Input\MouseInput.h"
#include "Input\KeyBoardInput.h"
#include <Xinput.h>
#include <Windows.h>
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dinput8.lib")

DWORD WINAPI FakeXInputGetState(DWORD dwUserIndex, XINPUT_STATE *pState)
{
	return ERROR_DEVICE_NOT_CONNECTED;
}

typedef DWORD WINAPI TF_XInputGetState(DWORD dwUserIndex, XINPUT_STATE *pState);
TF_XInputGetState *s_XInputGetState = FakeXInputGetState;


InputMapping::CInputMapperImplementation::CInputMapperImplementation()
:m_MovementX(0)
,m_MovementY(0)
,m_MovementZ(0)
,m_ButtonLeft(false)
,m_ButtonMiddle(false)
,m_ButtonRight(false)
,m_PreviousButtonLeft(false)
,m_PreviousButtonMiddle(false)
,m_PreviousButtonRight(false)
,m_MouseSpeed(1.0f)
{
	
}

void InputMapping::CInputMapperImplementation::Initialize(const std::wstring &InputManagerContextsFileName, HWND Hwnd)
{
	m_Hwnd = Hwnd;

	unsigned l_Count;
	std::wifstream l_ContextFile(InputManagerContextsFileName);
	if (!(l_ContextFile >> l_Count)){ assert(false); }

	for (unsigned i = 0; i < l_Count; ++i)
	{
		std::wstring l_Name = ReadValue<std::wstring>(l_ContextFile);
		std::wstring l_File = ReadValue<std::wstring>(l_ContextFile);
		m_InputContexts[l_Name] = new CInputContext("Data\\MainInputContext.xml");
	}

	bool l_Result = InitializeDirectXInput();
	if (!l_Result)
	{
		assert(false);
	}
}

bool InputMapping::CInputMapperImplementation::InitializeDirectXInput()
{
	HRESULT l_HR;
	if (FAILED(l_HR = DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&m_DirectInput, NULL)))
		return false;
	if (FAILED(l_HR = m_DirectInput->CreateDevice(GUID_SysMouse, &m_DirectInputDevice, NULL)))
		return false;
	if (FAILED(l_HR = m_DirectInputDevice->SetDataFormat(&c_dfDIMouse2)))
		return false;
	//if (FAILED(l_HR = m_DirectInputDevice->SetCooperativeLevel(hWnd, l_CoopFlags)))

	HMODULE l_XInputLibrary = LoadLibraryA("xinput1_4.dll");
	if (!l_XInputLibrary) { l_XInputLibrary = LoadLibraryA("xinput9_1_0.dll"); }

	if (!l_XInputLibrary) { l_XInputLibrary = LoadLibraryA("xinput1_3.dll"); }

	if (l_XInputLibrary) 
	{
		s_XInputGetState = (TF_XInputGetState *)GetProcAddress(l_XInputLibrary, "XInputGetState");
		if (!s_XInputGetState) { s_XInputGetState = FakeXInputGetState; }
	}
	else assert(false);

	if (m_DirectInputDevice != NULL)
	{
		m_DirectInputDevice->Acquire();
		m_MouseInput = new CMouseInput();
	}

	m_KeyBoardInput = new CKeyBoardInput();

	return true;
}

CMouseInput* InputMapping::CInputMapperImplementation::GetMouse() const
{
	POINT l_Cursor;

	if (GetCursorPos(&l_Cursor))
	{
		if (ScreenToClient(m_Hwnd, &l_Cursor))
		{
			m_MouseInput->Update(l_Cursor.x, l_Cursor.y, m_ButtonLeft, !m_ButtonLeft);
		}
	}

	return m_MouseInput;
}

CKeyBoardInput* InputMapping::CInputMapperImplementation::GetKeyBoard() const
{
	return m_KeyBoardInput;
}

void InputMapping::CInputMapperImplementation::BeginFrame()
{
	m_DirectXCursor = Vect2i(0, 0);

	if (m_Focus)
	{
		{
			DIMOUSESTATE2 l_DIMouseState;

			if (m_DirectInputDevice != NULL)
			{

				ZeroMemory(&l_DIMouseState, sizeof(l_DIMouseState));
				HRESULT l_HR = m_DirectInputDevice->GetDeviceState(sizeof(DIMOUSESTATE2), &l_DIMouseState);
				if (FAILED(l_HR))
				{
					l_HR = m_DirectInputDevice->Acquire();
					while (l_HR == DIERR_INPUTLOST)
						l_HR = m_DirectInputDevice->Acquire();
				}
				else
				{
					m_MovementX = l_DIMouseState.lX;
					m_MovementY = l_DIMouseState.lY;
					m_MovementZ = l_DIMouseState.lZ;

					if (m_MovementX == 0 && m_MovementY == 0)
						printf("a\n");
					else
						printf("b\n");

					m_PreviousButtonLeft = m_ButtonLeft;
					m_PreviousButtonMiddle = m_ButtonMiddle;
					m_PreviousButtonRight = m_ButtonRight;

					m_ButtonRight = (l_DIMouseState.rgbButtons[1] & 0x80) != 0;
					m_ButtonLeft = (l_DIMouseState.rgbButtons[0] & 0x80) != 0;
					m_ButtonMiddle = (l_DIMouseState.rgbButtons[2] & 0x80) != 0;

					m_DirectXCursor = Vect2i(m_MovementX, m_MovementY);

					CEngine::GetSingleton().GetInputMapper()->SetOSAxisValue(InputMapping::OS_INPUT_AXIS_MOUSE_X, static_cast<double>(m_MovementX * m_MouseSpeed));
					CEngine::GetSingleton().GetInputMapper()->SetOSAxisValue(InputMapping::OS_INPUT_AXIS_MOUSE_Y, static_cast<double>(m_MovementY * m_MouseSpeed));

					CEngine::GetSingleton().GetInputMapper()->SetOSButtonState(InputMapping::SOSInputButtons(InputMapping::OS_INPUT_BUTTON_MOUSE_LEFT, false, false), m_ButtonLeft, m_PreviousButtonLeft);
					CEngine::GetSingleton().GetInputMapper()->SetOSButtonState(InputMapping::SOSInputButtons(InputMapping::OS_INPUT_BUTTON_MOUSE_RIGHT,false,false), m_ButtonRight, m_PreviousButtonRight);
					CEngine::GetSingleton().GetInputMapper()->SetOSButtonState(InputMapping::SOSInputButtons(InputMapping::OS_INPUT_BUTTON_MOUSE_WHEEL, false, false), m_ButtonMiddle, m_PreviousButtonMiddle);
				}
			}
		}

		XINPUT_STATE l_ControllerState[4];
		bool l_ControllerPresent[4] = { false, false, false, false };
		for (int i = 0; i < 4; ++i)
		{
			if (s_XInputGetState(i, &l_ControllerState[i]) == ERROR_SUCCESS)
			{
				l_ControllerPresent[i] = true;
			}
		}
	}
}

void InputMapping::CInputMapperImplementation::EndFrame()
{
	m_DirectXCursor = Vect2i(0, 0);
}


InputMapping::CInputMapperImplementation::~CInputMapperImplementation()
{
	for (std::map<std::wstring, CInputContext*>::iterator iter = m_InputContexts.begin(); iter != m_InputContexts.end(); ++iter)
	{
		delete iter->second;
	}
	ReleaseDirectXInput();
}

void InputMapping::CInputMapperImplementation::ReleaseDirectXInput()
{
	if (m_DirectInputDevice != NULL) m_DirectInputDevice->Unacquire();
	if (m_DirectInputDevice) { m_DirectInputDevice->Release(); m_DirectInputDevice = 0; }
	if (m_DirectInput) { m_DirectInput->Release(); m_DirectInput = 0; }
	if (m_MouseInput) { delete m_MouseInput; m_MouseInput = 0; }
	if (m_KeyBoardInput) { delete m_KeyBoardInput; m_KeyBoardInput = 0; }
}

void InputMapping::CInputMapperImplementation::SetOSButtonState(SOSInputButtons Button, bool Pressed, bool Previouslypressed)
{
	EAction action;
	EState state;

	if(Pressed && !Previouslypressed)
	{
		if(MapButtonToAction(Button, action))
		{
			m_CurrentMappedInput.Actions.insert(action);
			return;
		}
	}

	if(Pressed)
	{
		if(MapButtonToState(Button, state))
		{
			m_CurrentMappedInput.States.insert(state);
			return;
		}
	}

	MapAndEatButton(Button);
}

void InputMapping::CInputMapperImplementation::SetOSAxisValue(EOSInputAxis axis, double value)
{
	for (std::list<CInputContext*>::const_iterator iter = m_ActiveContexts.begin(); iter != m_ActiveContexts.end(); ++iter)
	{
		const CInputContext* context = *iter;

		ERange range;
		if(context->MapAxisToRange(axis, range))
		{
			m_CurrentMappedInput.Ranges[range] = context->GetConversions().Convert(range, value * context->GetSensitivity(range));
			break;
		}
	}
}


//
// Dispatch input to all registered callbacks
//
void InputMapping::CInputMapperImplementation::Dispatch() const
{
	SMappedInput input = m_CurrentMappedInput;
	for (std::multimap<int, InputCallback>::const_iterator iter = m_CallbackTable.begin(); iter != m_CallbackTable.end(); ++iter)
		(*iter->second)(input);
}

//Clears actions and ranges
void InputMapping::CInputMapperImplementation::Clear()
{
	m_CurrentMappedInput.Actions.clear();
	m_CurrentMappedInput.Ranges.clear();
	// Note: we do NOT clear states, because they need to remain set
	// across frames so that they don't accidentally show "off" for
	// a tick or two while the raw input is still pending.
}

//
// Add a callback to the dispatch table
//
void InputMapping::CInputMapperImplementation::AddCallback(InputCallback callback, int priority)
{
	m_CallbackTable.insert(std::make_pair(priority, callback));
}


//
// Push an active input context onto the stack
//
void InputMapping::CInputMapperImplementation::PushContext(const std::wstring& name)
{
	std::map<std::wstring, CInputContext*>::iterator iter = m_InputContexts.find(name);
	if (iter == m_InputContexts.end())
		throw std::exception("Invalid input context pushed");

	m_ActiveContexts.push_front(iter->second);
}

//
// Pop the current input context off the stack
//
void InputMapping::CInputMapperImplementation::PopContext()
{
	if (m_ActiveContexts.empty())
		throw std::exception("Cannot pop input context, no contexts active!");

	m_ActiveContexts.pop_front();
}


//
// Helper: map a button to an action in the active context(s)
//
bool InputMapping::CInputMapperImplementation::MapButtonToAction(SOSInputButtons Button, EAction& Action) const
{
	for (std::list<CInputContext*>::const_iterator l_It = m_ActiveContexts.begin(); l_It != m_ActiveContexts.end(); ++l_It)
	{
		const CInputContext* context = *l_It;

		if(context->MapButtonToAction(Button, Action))
			return true;
	}

	return false;
}

//
// Helper: map a button to a state in the active context(s)
//
bool InputMapping::CInputMapperImplementation::MapButtonToState(SOSInputButtons Button, EState& State) const
{
	for (std::list<CInputContext*>::const_iterator iter = m_ActiveContexts.begin(); iter != m_ActiveContexts.end(); ++iter)
	{
		const CInputContext* context = *iter;

		if(context->MapButtonToState(Button, State))
			return true;
	}

	return false;
}

//
// Helper: eat all input mapped to a given button
//
void InputMapping::CInputMapperImplementation::MapAndEatButton(SOSInputButtons Button)
{
	EAction l_Action;
	EState l_State;

	if (MapButtonToAction(Button, l_Action))
		m_CurrentMappedInput.EatAction(l_Action);

	if (MapButtonToState(Button, l_State))
		m_CurrentMappedInput.EatState(l_State);
}

