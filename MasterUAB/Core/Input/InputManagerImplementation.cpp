#include <Windows.h>
#include <Xinput.h>

#include "Input\InputManagerImplementation.h"

#include "XML\XMLTreeNode.h"

#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dinput8.lib")

DWORD WINAPI FakeXInputGetState(DWORD dwUserIndex, XINPUT_STATE *pState)
{
	return ERROR_DEVICE_NOT_CONNECTED;
}

typedef DWORD WINAPI TF_XInputGetState(DWORD dwUserIndex, XINPUT_STATE *pState);
TF_XInputGetState *s_XInputGetState = FakeXInputGetState;

CInputManagerImplementation::CInputManagerImplementation()
:m_MovementX(0)
,m_MovementY(0)
,m_MovementZ(0)
,m_ButtonLeft(false)
,m_ButtonMiddle(false)
,m_ButtonRight(false)
,m_PreviousButtonLeft(false)
,m_PreviousButtonMiddle(false)
,m_PreviousButtonRight(false)
,m_MouseSpeed(1)
{
	m_Alt = false;
	m_Ctrl = false;

	for (int i = 0; i < 256; ++i)
	{
		m_KeysPrevious[i] = m_KeysCurrent[i] = false;
	}

	for (int i = 0; i < 4; ++i)
	{
		m_PadButtensPrevious[i] = 0;
	}
	

	// mouse input
	HRESULT l_HR;
	DWORD l_CoopFlags = 0;

	/*l_CoopFlags= DISCL_EXCLUSIVE | DISCL_FOREGROUND;*/

	if (FAILED(l_HR = DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&m_DI, NULL)))
		return;
	if (FAILED(l_HR = m_DI->CreateDevice(GUID_SysMouse, &m_Mouse, NULL)))
		return;
	if (FAILED(l_HR = m_Mouse->SetDataFormat(&c_dfDIMouse2)))
		return;
	//if (FAILED(l_HR = m_Mouse->SetCooperativeLevel(hWnd, l_CoopFlags)))
	//	return;

	HMODULE XInputLibrary = LoadLibraryA("xinput1_4.dll");
	if (!XInputLibrary)
	{
		XInputLibrary = LoadLibraryA("xinput9_1_0.dll");
	}

	if (!XInputLibrary)
	{
		XInputLibrary = LoadLibraryA("xinput1_3.dll");
	}

	if (XInputLibrary)
	{
		s_XInputGetState = (TF_XInputGetState *)GetProcAddress(XInputLibrary, "XInputGetState");
		if (!s_XInputGetState) { s_XInputGetState = FakeXInputGetState; }

	}

	if (m_Mouse != NULL)
	{
		m_Mouse->Acquire();
		m_MouseInput = new CMouseInput();
	}

	m_KeyBoardInput = new CKeyBoardInput();
	//else
		//MessageBox(hWnd, "Problem with de mouse input!", "Mouse", MB_ICONERROR | MB_OK);
}

void CInputManagerImplementation::Initialize(HWND Hwnd)
{
	m_HWND = Hwnd;
}

CInputManagerImplementation::~CInputManagerImplementation()
{
	if (m_Mouse != NULL)
		m_Mouse->Unacquire();

	CHECKED_RELEASE(m_Mouse);
	CHECKED_RELEASE(m_DI);
	CHECKED_DELETE(m_MouseInput);
	CHECKED_DELETE(m_KeyBoardInput);
}

CMouseInput* CInputManagerImplementation::GetMouse() const
{
	POINT l_Cursor;
	
	if (GetCursorPos(&l_Cursor))
	{
		if (ScreenToClient(m_HWND, &l_Cursor))
		{
			m_MouseInput->Update(l_Cursor.x, l_Cursor.y, m_ButtonLeft,  !m_ButtonLeft);
		}
	}

	return m_MouseInput;
}

CKeyBoardInput* CInputManagerImplementation::GetKeyBoard() const
{
	return m_KeyBoardInput;
}

void CInputManagerImplementation::LoadCommandsFromFile(const std::string& path)
{
	m_Actions.clear();

	/*
	{
		Action action = { "STRAFE_LEFT", KEYBOARD, Action::WHILE_PRESSED };
		action.keyboard.key = 'A';
		action.keyboard.needsAlt = false;
		action.keyboard.needsCtrl = false;

		action.triggersAxis = true;
		action.axisName = "STRAFE";
		action.axisValue = -1;

		m_Actions.push_back(action);
	}
	{
		Action action = { "STRAFE_RIGHT", KEYBOARD, Action::WHILE_PRESSED };
		action.keyboard.key = 'D';
		action.keyboard.needsAlt = false;
		action.keyboard.needsCtrl = false;

		action.triggersAxis = true;
		action.axisName = "STRAFE";
		action.axisValue = 1;

		m_Actions.push_back(action);
	}
	{
		Action action = { "MOVE_FWD", KEYBOARD, Action::WHILE_PRESSED };
		action.keyboard.key = 'W';
		action.keyboard.needsAlt = false;
		action.keyboard.needsCtrl = false;

		action.triggersAxis = true;
		action.axisName = "MOVE_FWD";
		action.axisValue = 1;

		m_Actions.push_back(action);
	}
	{
		Action action = { "MOVE_BACK", KEYBOARD, Action::WHILE_PRESSED };
		action.keyboard.key = 'S';
		action.keyboard.needsAlt = false;
		action.keyboard.needsCtrl = false;

		action.triggersAxis = true;
		action.axisName = "MOVE_FWD";
		action.axisValue = -1;

		m_Actions.push_back(action);
	}

	{
		Axis axis = { "X_AXIS", MOUSE };
		axis.mouse.axis = MouseAxis::X;
		axis.mouse.scale = 0.05f;

		m_Axis.push_back(axis);
	}
	{
		Axis axis = { "Y_AXIS", MOUSE };
		axis.mouse.axis = MouseAxis::Y;
		axis.mouse.scale = 0.01f;

		m_Axis.push_back(axis);
	}
	{
		Axis axis = { "ZOOM", MOUSE };
		axis.mouse.axis = MouseAxis::WHEEL;
		axis.mouse.scale = 0.05f;

		m_Axis.push_back(axis);
	}


	{
		Axis axis = { "X_AXIS", GAMEPAD };
		axis.gamepad.axis = GamepadAxis::LEFT_X;
		axis.gamepad.gamepadNumber = 0;

		m_Axis.push_back(axis);
	}
	{
		Axis axis = { "Y_AXIS", GAMEPAD };
		axis.gamepad.axis = GamepadAxis::LEFT_Y;
		axis.gamepad.gamepadNumber = 0;

		m_Axis.push_back(axis);
	}*/
	
	CXMLTreeNode l_XML;
	if (l_XML.LoadFile(path.c_str()))
	{
		CXMLTreeNode l_Input = l_XML["input"];
		if (l_Input.Exists())
		{
			for (int i = 0; i < l_Input.GetNumChildren(); ++i)
			{
				CXMLTreeNode l_Element = l_Input(i);

				if (l_Element.GetName() == std::string("action"))
				{
					CXMLTreeNode &l_Action = l_Element;

					Action action = {};
					action.name = l_Action.GetPszProperty("name");
					std::string type = l_Action.GetPszProperty("type", "KEYBOARD");
					action.inputType = ParseInputType(type);
					action.mode = ParseMode(l_Action.GetPszProperty("mode", "ON_PRESS", false));

					action.triggersAxis = l_Action.GetBoolProperty("triggers_axis", false, false);
					action.axisName = l_Action.GetPszProperty("axis", "", false);
					action.axisValue = l_Action.GetFloatProperty("axis_value", 1, false);

					switch (action.inputType)
					{
						case KEYBOARD:
						{
							std::string l_S = l_Action.GetPszProperty("key");
							if (l_S.size() > 1)
								action.keyboard.key = GetAsciiCode(l_S);
							else 
								action.keyboard.key = l_Action.GetPszProperty("key")[0];
							action.keyboard.needsAlt = l_Action.GetBoolProperty("needs_alt", false, false);
							action.keyboard.needsCtrl = l_Action.GetBoolProperty("needs_ctrl", false, false);
							break;
						}
						case MOUSE:
							//action.mouse.button = l_Action.GetPszProperty("button")[0];
							//break;

						case GAMEPAD:
							//action.gamepad.button = l_Action.GetPszProperty("button", "RIGHT")[0];
							//action.gamepad.gamepadNumber
							break;
							// TODO: Mouse y Gamepad

						// Pista: para parsear botones del gamepad, usad las constantes:
						//   podeis tener más de una a la vez usando "XINPUT_GAMEPAD_A | XINPUT_GAMEPAD_B", por ejemplo.
						//   pero para que eso funcione bién con ON_PRESS tendréis que mejorar la lógica de esta clase o ser frame perfect pulsando botones

						//  XINPUT_GAMEPAD_DPAD_UP       
						//  XINPUT_GAMEPAD_DPAD_DOWN     
						//  XINPUT_GAMEPAD_DPAD_LEFT     
						//  XINPUT_GAMEPAD_DPAD_RIGHT    
						//  XINPUT_GAMEPAD_START         
						//  XINPUT_GAMEPAD_BACK          
						//  XINPUT_GAMEPAD_LEFT_THUMB    
						//  XINPUT_GAMEPAD_RIGHT_THUMB   
						//  XINPUT_GAMEPAD_LEFT_SHOULDER 
						//  XINPUT_GAMEPAD_RIGHT_SHOULDER
						//  XINPUT_GAMEPAD_A             
						//  XINPUT_GAMEPAD_B             
						//  XINPUT_GAMEPAD_X             
						//  XINPUT_GAMEPAD_Y             
						// 
					}

					m_Actions.push_back(action);
				}
				else if (l_Element.GetName() == std::string("axis"))
				{
					CXMLTreeNode &l_Axis = l_Element;
					
					Axis axis = {};
					axis.name = l_Axis.GetPszProperty("name");
					std::string type = l_Axis.GetPszProperty("type", "");
					axis.inputType = ParseInputType(type);
			
					switch (axis.inputType)
					{
						case MOUSE:
							axis.mouse.axis=(MouseAxis)l_Axis.GetIntProperty("axis");
							axis.mouse.scale=l_Axis.GetFloatProperty("scale");
							break;
							
						case GAMEPAD:
							//axis.gamepad.axis=GamepadAxis::;
							//axis.gamepad.gamepadNumber=0.05;
							break;
					}

					m_Axis.push_back(axis);
				}

			}
		}
	}

	EndFrame();
}

uint8_t CInputManagerImplementation::GetAsciiCode(const std::string& Characters)
{
	uint8_t l_Return;

	if (Characters == "ESC")
	{
		l_Return = 0x1B;
	}

	return l_Return;
}

CInputManagerImplementation::Action::Mode CInputManagerImplementation::ParseMode(const std::string& mode)
{
	if (mode == "ON_PRESS")
	{
		return Action::ON_PRESS;
	}
	else if (mode == "ON_RELEASE")
	{
		return Action::ON_RELEASE;
	}
	else if (mode == "WHILE_PRESSED")
	{
		return Action::WHILE_PRESSED;
	}
	else if (mode == "WHILE_RELEASED")
	{
		return Action::WHILE_RELEASED;
	}
	else
	{
		return (Action::Mode) - 1;
	}
}

CInputManagerImplementation::InputType CInputManagerImplementation::ParseInputType(const std::string& inputType)
{
	if (inputType == "KEYBOARD")
	{
		return KEYBOARD;
	}
	else if (inputType == "MOUSE")
	{
		return MOUSE;
	}
	else if (inputType == "GAMEPAD")
	{
		return GAMEPAD;
	}
	else
	{
		return (InputType) - 1;
	}
}

void CInputManagerImplementation::BeginFrame()
{
	m_ActiveActions.clear();
	m_ActiveAxis.clear();
	m_CursorD = Vect2i(0, 0);

	if (m_Focus)
	{
		{
			DIMOUSESTATE2 l_DIMouseState;

			if (m_Mouse != NULL)
			{

				ZeroMemory(&l_DIMouseState, sizeof(l_DIMouseState));
				HRESULT l_HR = m_Mouse->GetDeviceState(sizeof(DIMOUSESTATE2), &l_DIMouseState);
				if (FAILED(l_HR))
				{
					l_HR = m_Mouse->Acquire();
					while (l_HR == DIERR_INPUTLOST)
						l_HR = m_Mouse->Acquire();
				}
				else
				{

					m_MovementX = l_DIMouseState.lX;
					m_MovementY = l_DIMouseState.lY;
					m_MovementZ = l_DIMouseState.lZ;

					if(m_MovementX == 0 && m_MovementY==0)
						printf("a\n");
					else
						printf("b\n");


					m_PreviousButtonLeft = m_ButtonLeft;
					m_PreviousButtonMiddle = m_ButtonMiddle;
					m_PreviousButtonRight = m_ButtonRight;

					m_ButtonRight = (l_DIMouseState.rgbButtons[1] & 0x80) != 0;
					m_ButtonLeft = (l_DIMouseState.rgbButtons[0] & 0x80) != 0;
					m_ButtonMiddle = (l_DIMouseState.rgbButtons[2] & 0x80) != 0;

					m_CursorD = Vect2i(m_MovementX, m_MovementY);
				}
			}
		}

		// ----------------------------

		XINPUT_STATE l_ControllerState[4];
		bool l_ControllerPresent[4] = { false, false, false, false };
		for (int i = 0; i < 4; ++i)
		{
			if (s_XInputGetState(i, &l_ControllerState[i]) == ERROR_SUCCESS)
			{
				l_ControllerPresent[i] = true;
			}
		}

		// ----------------------------


		for (std::vector<Action>::const_iterator it = m_Actions.cbegin(); it != m_Actions.cend(); ++it)
		{
			const Action& action = *it;

			bool current = false, previous = false;
			bool otherNeeds = true;
			bool isActionActive = false;

			switch (action.inputType)
			{
			case KEYBOARD:
				current = m_KeysCurrent[action.keyboard.key];
				previous = m_KeysPrevious[action.keyboard.key];
				otherNeeds = (!action.keyboard.needsAlt || m_Alt) && (!action.keyboard.needsCtrl || m_Ctrl);
				break;

			case MOUSE:
				switch (action.mouse.button)
				{
				case Action::LEFT:
					current = m_ButtonLeft;
					previous = m_PreviousButtonLeft;
					break;

				case Action::CENTER:
					current = m_ButtonMiddle;
					previous = m_PreviousButtonMiddle;
					break;

				case Action::RIGHT:
					current = m_ButtonRight;
					previous = m_PreviousButtonRight;
					break;

				default:
					break;
				}
				break;

			case GAMEPAD:
				if (l_ControllerPresent[action.gamepad.gamepadNumber])
				{
					WORD rawState = l_ControllerState[action.gamepad.gamepadNumber].Gamepad.wButtons;
					current = ((rawState & action.gamepad.button) != 0);
					previous = ((m_PadButtensPrevious[action.gamepad.gamepadNumber] & action.gamepad.button) != 0);
				}
				break;

			default:
				break;
			}


			switch (action.mode)
			{
			case Action::ON_PRESS:

				if (otherNeeds && current && !previous)
				{
					isActionActive = true;
				}
				break;

			case Action::WHILE_PRESSED:

				if (otherNeeds && current)
				{
					isActionActive = true;
				}
				break;

			case Action::ON_RELEASE:

				// TODO: añadir acciones de release
				if (otherNeeds && !current && previous)
				{
					isActionActive = true;
				}
				break;

			case Action::WHILE_RELEASED:
				// TODO: añadir acciones de botón no pulsado

				if (otherNeeds && !current)
				{
					isActionActive = true;
				}
				break;

			default:
				break;
			}

			if (isActionActive)
			{
				m_ActiveActions.insert(action.name);

				if (action.triggersAxis)
				{
					m_ActiveAxis[action.axisName] += action.axisValue;
				}
			}
		}

		// ----------------------------

		for (std::vector<Axis>::const_iterator it = m_Axis.cbegin(); it != m_Axis.cend(); ++it)
		{
			const Axis& axis = *it;
			switch (axis.inputType)
			{
			case MOUSE:
				switch (axis.mouse.axis)
				{
				case X:
					m_ActiveAxis[axis.name] += m_MovementX * m_MouseSpeed * axis.mouse.scale;
					break;
				case Y:
					m_ActiveAxis[axis.name] += m_MovementY * m_MouseSpeed * axis.mouse.scale;
					break;
				case WHEEL:
					m_ActiveAxis[axis.name] += m_MovementZ * axis.mouse.scale;
					break;
				}
				break;
			case GAMEPAD:
				switch (axis.gamepad.axis)
				{
				case LEFT_X:
					if (l_ControllerPresent[axis.gamepad.gamepadNumber])
					{
						SHORT rawState = l_ControllerState[axis.gamepad.gamepadNumber].Gamepad.sThumbLX;
						if (rawState < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
						{
							m_ActiveAxis[axis.name] += (float)((rawState + XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) / (32768.0f - XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE));
						}
						else if (rawState > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
						{
							m_ActiveAxis[axis.name] += (float)((rawState - XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) / (32768.0f - XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE));
						}
					}
					break;
				case LEFT_Y:
					if (l_ControllerPresent[axis.gamepad.gamepadNumber])
					{
						SHORT rawState = l_ControllerState[axis.gamepad.gamepadNumber].Gamepad.sThumbLY;
						if (rawState < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
						{
							m_ActiveAxis[axis.name] += (float)((rawState + XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) / (32768.0f - XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE));
						}
						else if (rawState > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
						{
							m_ActiveAxis[axis.name] += (float)((rawState - XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) / (32768.0f - XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE));
						}
					}
					break;
				case RIGHT_X:
					if (l_ControllerPresent[axis.gamepad.gamepadNumber])
					{
						SHORT rawState = l_ControllerState[axis.gamepad.gamepadNumber].Gamepad.sThumbRX;
						if (rawState < -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
						{
							m_ActiveAxis[axis.name] += (float)((rawState + XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) / (32768.0f - XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE));
						}
						else if (rawState > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
						{
							m_ActiveAxis[axis.name] += (float)((rawState - XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) / (32768.0f - XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE));
						}
					}
					break;
	
				case RIGHT_Y:
					if (l_ControllerPresent[axis.gamepad.gamepadNumber])
					{
						SHORT rawState = l_ControllerState[axis.gamepad.gamepadNumber].Gamepad.sThumbRY;
						if (rawState < -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
						{
							m_ActiveAxis[axis.name] += (float)((rawState + XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) / (32768.0f - XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE));
						}
						else if (rawState > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
						{
							m_ActiveAxis[axis.name] += (float)((rawState - XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) / (32768.0f - XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE));
						}
					}
					break;
				case LEFT_TRIGGER:
					if (l_ControllerPresent[axis.gamepad.gamepadNumber])
					{
						BYTE rawState = l_ControllerState[axis.gamepad.gamepadNumber].Gamepad.bLeftTrigger;
						if (rawState > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
						{
							m_ActiveAxis[axis.name] += (float)((rawState + XINPUT_GAMEPAD_TRIGGER_THRESHOLD) / (256.0f - XINPUT_GAMEPAD_TRIGGER_THRESHOLD));
						}
					}
					break;
				case RIGHT_TRIGGER:
					// TODO: Controles para el trigger derecho
					break;

				}
				break;
			}

		}

		// Save gamepad button states

		{

			for (int i = 0; i < 4; ++i)
			{
				if (l_ControllerPresent[i])
				{
					m_PadButtensPrevious[i] = l_ControllerState[i].Gamepad.wButtons;
				}
				else
				{
					m_PadButtensPrevious[i] = 0;
				}
			}
		}
	}
}

void CInputManagerImplementation::EndFrame()
{
	for (int i = 0; i < 256; ++i)
	{
		m_KeysPrevious[i] = m_KeysCurrent[i];
	}

	m_Alt = m_Ctrl = false;

	m_CursorD = Vect2i(0, 0);
}

bool CInputManagerImplementation::KeyEventReceived(unsigned int wParam, unsigned int lParam)
{
	bool IsDown = ((lParam & (1 << 31)) == 0);
	m_Alt = ((lParam & (1 << 29)) != 0);

	m_KeysCurrent[wParam] = IsDown;

	return false;
}
