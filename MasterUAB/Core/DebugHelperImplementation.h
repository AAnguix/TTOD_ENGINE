#ifndef _DEBUGHELPERIMPLEMENTATION_H
#define _DEBUGHELPERIMPLEMENTATION_H

#include <Windows.h>

#include <unordered_map>

#include <AntTweakBar.h>
#include "DebugHelper.h"

class CEmptyPointerClass;

class CDebugHelperImplementation : public CDebugHelper
{
public:
	CDebugHelperImplementation();
	~CDebugHelperImplementation();
	
	void Initialize(void *device);

	bool Update(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	virtual void Render();

	virtual void Log(const std::string& text) const;
	virtual void RegisterBar(const SDebugBar& bar);
	

	std::unordered_map<std::string, TwBar*> m_Bars;
	TwType m_PosRotType;
	TwType m_PosType;
	TwType m_Vector2f;
	TwType m_Vector3f;
	TwType m_Vector4f;

	void CreateBar(const std::string BarName);
	virtual void RemoveBar();
	void ResetButtons();
	
	static void TW_CALL CallLuaFunction(void *ClientData);
	
	void RegisterLUAButton(const std::string &ButtonName, const std::string &Script);

	
	void RegisterBoolParameter(const std::string &ParameterName, CEmptyPointerClass *Data, const std::string &Description);
	void RegisterInt32Parameter(const std::string &ParameterName, CEmptyPointerClass *Data, const std::string &Description);
	void RegisterFloatParameter(const std::string &ParameterName, CEmptyPointerClass *Data, const std::string &Description);
	void RegisterVect2fParameter(const std::string &ParameterName, CEmptyPointerClass *Data, const std::string &Description);
	void RegisterVect3fParameter(const std::string &ParameterName, CEmptyPointerClass *Data, const std::string &Description);
	void RegisterVect4fParameter(const std::string &ParameterName, CEmptyPointerClass *Data, const std::string &Description);
	
	void RegisterColorParameter(const std::string &ParameterName, CEmptyPointerClass *Data, const std::string &Description);
	void RegisterColor32Parameter(const std::string &ParameterName, CEmptyPointerClass *Data, const std::string &Description);
	void RegisterStringParameter(const std::string &ParameterName, CEmptyPointerClass *Data, const std::string &Description);
	void RegisterPositionOrientationParameter(const std::string &ParameterName, CEmptyPointerClass *Data, const std::string &Description);

private:
	void RegisterParameter(TwType ParameterType,const std::string &ParameterName, CEmptyPointerClass *Data, const std::string &Description);
	void RegisterVariable(SDebugVariable Variable,TwType BarType,const char* Params);
	std::vector<std::string> m_LuaParameters;
	std::string m_CurrentBarName;
};

#endif