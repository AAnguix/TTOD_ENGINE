#ifndef _INPUTMAPPERIMPLEMENTATION
#define _INPUTMAPPERIMPLEMENTATION_H

#include "Input\OSInputConstants.h"
#include "Input\InputConstants.h"
#include <map>
#include <set>
#include <list>
#include <string>

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include "Input\InputMapper.h"
class CMouseInput;
class CKeyBoardInput;

namespace InputMapping
{
	//Forward
	class CInputContext;

	//Stores the current input configuration
	struct SMappedInput
	{
		std::set<EAction> Actions;
		std::set<EState> States;
		std::map<ERange, double> Ranges;

		void EatAction(EAction Action)	{ Actions.erase(Action); }
		void EatState(EState State)	{ States.erase(State); }
		void EatRange(ERange Range)
		{
			std::map<ERange, double>::iterator iter = Ranges.find(Range);
			if(iter != Ranges.end())
				Ranges.erase(iter);
		}
	};

	typedef void (*InputCallback)(SMappedInput& Inputs);


	class CInputMapperImplementation : public CInputMapper
	{

	public:
		CInputMapperImplementation();
		~CInputMapperImplementation();
		void Initialize(HWND Hwnd);

	public:
		void Clear();
		void SetOSButtonState(SOSInputButtons Button, bool Pressed, bool Previouslypressed);
		float SetOSAxisValue(EOSInputAxis axis, double value);

	public:
		void Dispatch() const;

	public:
		void BeginFrame();
		void EndFrame();
		void SetFocus(bool Focus) { m_Focus = Focus; }

	//Callback registration interface
	public:
		void AddCallback(InputCallback callback, int priority);

	//Context management interface
	public:
		void PushContext(const std::wstring& name);
		void PopContext();

	//Data used by the GUI
	public:
		CMouseInput* GetMouse() const;
		CKeyBoardInput* GetKeyBoard() const;
		HWND GetHwnd() const { return m_Hwnd; };

		int GetMovementX() const { return m_MovementX; };
		int GetMovementY() const { return m_MovementY; };

	private:
		bool MapButtonToAction(SOSInputButtons Button, EAction& ction) const;
		bool MapButtonToState(SOSInputButtons Button, EState& State) const;
		void MapAndEatButton(SOSInputButtons Button);

		bool InitializeDirectXInput();
		void ReleaseDirectXInput();

	private:
		
		std::map<std::wstring, CInputContext*> m_InputContexts;
		std::list<CInputContext*> m_ActiveContexts;
		std::multimap<int, InputCallback> m_CallbackTable;

		SMappedInput m_CurrentMappedInput;

		HWND m_Hwnd;
		CMouseInput* m_MouseInput;
		CKeyBoardInput* m_KeyBoardInput;

		//Mouse data
		int m_MovementX;
		int m_MovementY;
		int m_MovementZ;
		bool m_ButtonRight, m_PreviousButtonRight;
		bool m_ButtonLeft, m_PreviousButtonLeft;
		bool m_ButtonMiddle, m_PreviousButtonMiddle;

		//DirectX mouse
		LPDIRECTINPUT8 m_DirectInput;
		LPDIRECTINPUTDEVICE8 m_DirectInputDevice;

		float GetZBlurConstant(float Base, float XDisplacement, float ScreenWidth, float Thresold) const;
	};

}

#endif

