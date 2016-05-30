#ifndef _DEBUGHELPERIMPLEMENTATION_H
#define _DEBUGHELPERIMPLEMENTATION_H

#include <Windows.h>

#include <unordered_map>

#include <AntTweakBar.h>
#include "DebugHelper.h"

class CEmptyPointerClass;
class CMaterial;

class CDebugHelperImplementation : public CDebugHelper
{
public:
	struct ClientData
	{
		std::string m_Function;
		CEmptyPointerClass* m_Object;
		std::string m_ObjectType;
		ClientData(std::string Function, CEmptyPointerClass* Object, std::string ObjectType);
	};
	struct ChangeTextureClientData
	{
		std::string m_Function;
		CMaterial* m_Material;
		CEmptyPointerClass* m_Texture;
		unsigned int m_Index;
		ChangeTextureClientData(const std::string Function, CMaterial* Material, CEmptyPointerClass* Texture, unsigned int Index);
	};
	
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
	static void TW_CALL CallLuaExtendedFunction(void *ClientData);
	static void TW_CALL CallLuaFunctionChangeTexture(void *ChangeTextureClientData);
	
	static void TW_CALL CopyStdStringToClient(std::string& DestinationClientString, const std::string& SourceLibraryString);

	void RegisterLUAButton(const std::string &ButtonName, const std::string &Script);
	void RegisterLUAExtendedButton(const std::string &ButtonName, const std::string &Function, CEmptyPointerClass* Object, const std::string &ObjectType);
	void RegisterLUAChangeTextureButton(const std::string &ButtonName, const std::string &Function, CMaterial* Material, CEmptyPointerClass* NewTexture, unsigned int Index);

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
	std::vector<std::string*> m_LuaParameters;
	std::vector<ClientData*> m_LuaExtendedParameters;
	std::vector<ChangeTextureClientData*> m_LuaChangeTextureParameters;
	std::string m_CurrentBarName;
};

#endif