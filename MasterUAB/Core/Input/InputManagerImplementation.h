#ifndef _INPUTMANAGERIMPLEMENTATION_H
#define _INPUTMANAGERIMPLEMENTATION_H

#define DIRECTINPUT_VERSION 0x0800

#include <dinput.h>
#include <vector>
#include "Input\InputManager.h"
class CMouseInput;
class CKeyBoardInput;

class CInputManagerImplementation : public CInputManager
{
private: 
	uint8_t GetAsciiCode(const std::string& Characters);

public:
	CInputManagerImplementation();
	~CInputManagerImplementation();

	void Initialize(HWND Hwnd);

	void LoadCommandsFromFile(const std::string& path);

	void BeginFrame();
	void EndFrame();

	void SetFocus(bool focus) { m_Focus = focus; }
	void SetMouseSpeed(float _MouseSpeed) { m_MouseSpeed = _MouseSpeed; }

	bool KeyEventReceived(unsigned int wParam, unsigned int lParam);
	void UpdateCursor(int x, int y) { m_Cursor.x = x; m_Cursor.y = y; }
	void UpdateCursorMovement(int x, int y) { m_CursorD.x += x; m_CursorD.y += y; }

	CMouseInput* GetMouse() const;
	CKeyBoardInput* GetKeyBoard() const;
	HWND GetHwnd() const { return m_HWND; };

private:

	HWND m_HWND;

	enum InputType {
		KEYBOARD,
		MOUSE,
		GAMEPAD
	};

	struct Action
	{
		std::string name;

		enum Mode {
			ON_PRESS,
			ON_RELEASE,
			WHILE_PRESSED,
			WHILE_RELEASED
		};

		enum MouseButton
		{
			LEFT,
			CENTER,
			RIGHT
		};

		InputType inputType;
		Mode mode;
		union 
		{
			struct Keyboard 
			{
				uint8_t key;
				bool needsAlt;
				bool needsCtrl;
			} keyboard;

			struct Mouse 
			{
				MouseButton button;
			} mouse;

			struct Gamepad 
			{
				unsigned short button;
				uint8_t gamepadNumber;
			} gamepad;
		};
		bool triggersAxis;
		std::string axisName;
		float axisValue;
	};

	enum MouseAxis 
	{
		X,
		Y,
		WHEEL
	};

	enum GamepadAxis 
	{
		LEFT_X,
		LEFT_Y,
		RIGHT_X,
		RIGHT_Y,
		LEFT_TRIGGER,
		RIGHT_TRIGGER
	};

	struct Axis
	{
		std::string name;

		InputType inputType;
		union 
		{
			struct Mouse 
			{
				MouseAxis axis;
				float scale;
			} mouse;
			struct Gamepad 
			{
				GamepadAxis axis;
				uint8_t gamepadNumber;
			} gamepad;
		};
	};


	Action::Mode ParseMode(const std::string& mode);
	InputType ParseInputType(const std::string& inputType);
	Action::MouseButton CInputManagerImplementation::ParseMouseButton(const std::string& MouseButton);

	bool m_KeysCurrent[256], m_KeysPrevious[256];
	bool m_Alt, m_Ctrl;

	unsigned short m_PadButtensPrevious[4];

	std::vector<Action> m_Actions;
	std::vector<Axis> m_Axis;

	float m_MouseSpeed;

	// mouse data

	LPDIRECTINPUT8 m_DI;
	LPDIRECTINPUTDEVICE8 m_Mouse;
	CMouseInput* m_MouseInput;
	CKeyBoardInput* m_KeyBoardInput;
	WPARAM m_LastChar;

	int m_MovementX;
	int m_MovementY;
	int m_MovementZ;
	bool m_ButtonRight, m_PreviousButtonRight;
	bool m_ButtonLeft, m_PreviousButtonLeft;
	bool m_ButtonMiddle, m_PreviousButtonMiddle;
};

#endif