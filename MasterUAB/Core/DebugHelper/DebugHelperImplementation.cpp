#include "DebugHelperImplementation.h"
#include <cassert>
#include "Engine.h"
#include "Log\Log.h"
#include "LuabindManager\LuabindManager.h"
#include <luabind/luabind.hpp>
#include "Particles\ParticleSystemType.h"
#include "RenderableObjects\RenderableObject.h"
#include "StaticMeshes\StaticMesh.h"
#include "SceneRendererCommands\SceneRendererCommand.h"
#include "Materials\Material.h"
#include "Components\LuaComponent.h"

CDebugHelperImplementation::CDebugHelperImplementation() : m_CurrentBarName("")
{
}

void CDebugHelperImplementation::Initialize(void *device)
{
	int status = TwInit(TW_DIRECT3D11, device);
	assert(status);
	{
		TwStructMember structMembers[] =
		{
			{ "x", TW_TYPE_FLOAT, offsetof(SPositionOrientation, Position) + offsetof(Vect3f, x), "step=0.5 precision=1" },
			{ "y", TW_TYPE_FLOAT, offsetof(SPositionOrientation, Position) + offsetof(Vect3f, y), "step=0.5 precision=1" },
			{ "z", TW_TYPE_FLOAT, offsetof(SPositionOrientation, Position) + offsetof(Vect3f, z), "step=0.5 precision=1" },
			{ "yaw", TW_TYPE_FLOAT, offsetof(SPositionOrientation, Yaw), "step=0.05 precision=2" },
			{ "pitch", TW_TYPE_FLOAT, offsetof(SPositionOrientation, Pitch), "step=0.05 precision=2" },
			{ "roll", TW_TYPE_FLOAT, offsetof(SPositionOrientation, Roll), "step=0.05 precision=2" }
		};

		m_PosRotType = TwDefineStruct("POSITION_ORIENTATION", structMembers, 6, sizeof(SPositionOrientation), nullptr, nullptr);

		TwStructMember l_v2fStruct[] =
		{
			{ "x", TW_TYPE_FLOAT, offsetof(Vect2f, x), "step=0.01 precision=1" },
			{ "y", TW_TYPE_FLOAT, offsetof(Vect2f, y), "step=0.01 precision=1" },
		};

		m_Vector2f = TwDefineStruct("VECTOR2F", l_v2fStruct, 2, sizeof(Vect2f), nullptr, nullptr);

		TwStructMember l_v3fStruct[] =
		{
			{ "x", TW_TYPE_FLOAT, offsetof(Vect3f, x), "step=0.01 precision=1" },
			{ "y", TW_TYPE_FLOAT, offsetof(Vect3f, y), "step=0.01 precision=1" },
			{ "z", TW_TYPE_FLOAT, offsetof(Vect3f, z), "step=0.01 precision=1" },
		};

		m_Vector3f = TwDefineStruct("VECTOR3F", l_v3fStruct, 3, sizeof(Vect3f), nullptr, nullptr);

		TwStructMember l_v4fStruct[] =
		{
			{ "x", TW_TYPE_FLOAT, offsetof(Vect4f, x), "step=0.01 precision=1" },
			{ "y", TW_TYPE_FLOAT, offsetof(Vect4f, y), "step=0.01 precision=1" },
			{ "z", TW_TYPE_FLOAT, offsetof(Vect4f, z), "step=0.01 precision=1" },
			{ "w", TW_TYPE_FLOAT, offsetof(Vect4f, w), "step=0.01 precision=1" },
		};

		m_Vector4f = TwDefineStruct("VECTOR4F", l_v4fStruct, 4, sizeof(Vect4f), nullptr, nullptr);
	}
}

CDebugHelperImplementation::~CDebugHelperImplementation()
{
	ResetButtons();
	int status = TwTerminate();
	assert(status);
}

void CDebugHelperImplementation::Log(const std::string& text) const
{
	CEngine::GetSingleton().GetLogManager()->Log("DebugHelper: "+text);
}

void CDebugHelperImplementation::Render()
{
	int status = TwDraw();
	if (!status)
	{
		const char* error = TwGetLastError();
		Log(error);
	}
}

bool CDebugHelperImplementation::Update(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if(TwEventWin(hWnd, msg, wParam, lParam))
		return true;
	return false;
}

void CDebugHelperImplementation::RegisterBar(const SDebugBar& bar)
{
	int status = 0;
	std::unordered_map<std::string, TwBar*>::iterator it = m_Bars.find(bar.name);
	if (it != m_Bars.end())
	{
		status = TwDeleteBar(it->second);
		assert(status);
	}

	TwBar* twBar = TwNewBar(bar.name.c_str());
	
	for (size_t i = 0; i < bar.variables.size(); ++i)
	{
		if (bar.variables[i].type == BUTTON)
		{
			status = TwAddButton(twBar, bar.variables[i].name.c_str(), bar.variables[i].callback, bar.variables[i].data, "");
			assert(status);
		}
		else
		{
			TwType type = TW_TYPE_FLOAT;
			const char* params = "";
			switch (bar.variables[i].type)
			{
			case BOOL:
				type = TW_TYPE_BOOLCPP;
				break;
			case FLOAT:
				type = TW_TYPE_FLOAT;
				break;
			case INT:
				type = TW_TYPE_INT32;
				break;
			case COLOR32:
				type = TW_TYPE_COLOR32;
				params = " coloralpha=true colororder=argb ";
				break;
			case COLOR:
				type = TW_TYPE_COLOR4F;
				break;
			case STRING:
				type = TW_TYPE_CSSTRING(100);
				break;

			case POSITION_ORIENTATION:
				type = m_PosRotType;
				break;

			default:
				break;
			}

			switch (bar.variables[i].mode)
			{
			case READ:
				status = TwAddVarRO(twBar, bar.variables[i].name.c_str(), type, bar.variables[i].ptr, params);
				assert(status);
				break;

			case READ_WRITE:
				status = TwAddVarRW(twBar, bar.variables[i].name.c_str(), type, bar.variables[i].ptr, params);
				assert(status);
				break;

			default:
				break;
			}
		}
	}

	m_Bars[bar.name] = twBar;
	
}

/*void CDebugHelperImplementation::RemoveBar(const std::string& Bar)
{
	int l_Status= 0;
	std::unordered_map<std::string, TwBar*>::iterator it = m_Bars.find(Bar);
	if (it != m_Bars.end())
	{
		l_Status = TwDeleteBar(it->second);
		assert(l_Status);
	}
}*/


CDebugHelperImplementation::ChangeTextureClientData::ChangeTextureClientData(const std::string Function, CMaterial* Material, CEmptyPointerClass* Texture, unsigned int Index)
:m_Function(Function), m_Texture(Texture), m_Material(Material), m_Index(Index)
{
}

CDebugHelperImplementation::ClientData::ClientData(std::string Function, CEmptyPointerClass* Object, std::string ObjectType)
:m_Function(Function), m_Object(Object), m_ObjectType(ObjectType)
{
}

void CDebugHelperImplementation::CreateBar(const std::string BarName)
{
	int status = 0;
	std::unordered_map<std::string, TwBar*>::iterator it = m_Bars.find(BarName);
	if (it != m_Bars.end())
	{
		status = TwDeleteBar(it->second);
		assert(status);
	}

	TwBar* twBar = TwNewBar(BarName.c_str());
	TwCopyStdStringToClientFunc(CopyStdStringToClient);

	m_Bars[BarName] = twBar;
	m_CurrentBarName=BarName;
}

void CDebugHelperImplementation::RemoveBar()
{
	int l_Status= 0;
	std::unordered_map<std::string, TwBar*>::iterator it = m_Bars.find(m_CurrentBarName);
	if (it != m_Bars.end())
	{
		l_Status = TwDeleteBar(it->second);
		m_Bars.erase(it);
		assert(l_Status);
	}
}

void TW_CALL CDebugHelperImplementation::CallLuaFunction(void *ClientData)
{
	//const char *l_Script=(const char *)ClientData;
	std::string* l_Script = (std::string*)ClientData;

	CEngine::GetSingleton().GetLuabindManager()->RunCode(*l_Script);
}

void TW_CALL CDebugHelperImplementation::CallLuaExtendedFunction(void *ClientData)
{
	CDebugHelperImplementation::ClientData* l_ClientData = (CDebugHelperImplementation::ClientData*)ClientData;
	
	std::string l_ObjectType = l_ClientData->m_ObjectType.c_str();

	if (l_ObjectType == "partycle")
	{
		luabind::call_function<void>(CEngine::GetSingleton().GetLuabindManager()->GetLuaState(), l_ClientData->m_Function.c_str(), (CParticleSystemType*)l_ClientData->m_Object);
	}
	else if (l_ObjectType  == "material")
	{
		luabind::call_function<void>(CEngine::GetSingleton().GetLuabindManager()->GetLuaState(), l_ClientData->m_Function.c_str(), (CMaterial*)l_ClientData->m_Object);
	}
	else if (l_ObjectType == "renderable_object")
	{
		luabind::call_function<void>(CEngine::GetSingleton().GetLuabindManager()->GetLuaState(), l_ClientData->m_Function.c_str(), (CRenderableObject*)l_ClientData->m_Object);
	}
	else if (l_ObjectType == "static_mesh")
	{
		luabind::call_function<void>(CEngine::GetSingleton().GetLuabindManager()->GetLuaState(), l_ClientData->m_Function.c_str(), (CStaticMesh*)l_ClientData->m_Object);
	}
	else if (l_ObjectType == "scene_renderer_command")
	{
		luabind::call_function<void>(CEngine::GetSingleton().GetLuabindManager()->GetLuaState(), l_ClientData->m_Function.c_str(), (CSceneRendererCommand*)l_ClientData->m_Object);
	}
	else if (l_ObjectType == "lua_component")
	{
		luabind::call_function<void>(CEngine::GetSingleton().GetLuabindManager()->GetLuaState(), l_ClientData->m_Function.c_str(), (CLUAComponent*)l_ClientData->m_Object);
	}
	else
	{	
		#ifdef _DEBUG
			CEngine::GetSingleton().GetLogManager()->Log("Object type " + l_ObjectType + " isn't defined. Unable to call function " + l_ClientData->m_Function+" from extended button");
		#endif
	}

}

void TW_CALL CDebugHelperImplementation::CallLuaFunctionChangeTexture(void *ChangeTextureClientData)
{
	CDebugHelperImplementation::ChangeTextureClientData* l_ClientData = (CDebugHelperImplementation::ChangeTextureClientData*)ChangeTextureClientData;

	std::string* l_Texture = (std::string*)l_ClientData->m_Texture;
																																						//l_Texture->c_str()
	luabind::call_function<void>(CEngine::GetSingleton().GetLuabindManager()->GetLuaState(), l_ClientData->m_Function.c_str(), l_ClientData->m_Material, l_Texture->c_str(), l_ClientData->m_Index);
}

void TW_CALL CDebugHelperImplementation::CopyStdStringToClient(std::string& DestinationClientString, const std::string& SourceLibraryString)
{
	DestinationClientString = SourceLibraryString;
}

void CDebugHelperImplementation::ResetButtons()
{
	for (size_t i = 0; i < m_LuaParameters.size(); ++i)
	{
		if (m_LuaParameters[i] != NULL) delete(m_LuaParameters[i]); m_LuaParameters[i] = NULL;
	}
	for (size_t i = 0; i < m_LuaExtendedParameters.size(); ++i)
	{
		if (m_LuaExtendedParameters[i] != NULL) delete(m_LuaExtendedParameters[i]); m_LuaExtendedParameters[i] = NULL;
	}
	for (size_t i = 0; i < m_LuaChangeTextureParameters.size(); ++i)
	{
		if (m_LuaChangeTextureParameters[i] != NULL) delete(m_LuaChangeTextureParameters[i]); m_LuaChangeTextureParameters[i] = NULL;
	}
	m_LuaParameters.clear();
	m_LuaChangeTextureParameters.clear();
	m_LuaExtendedParameters.clear();
}

void CDebugHelperImplementation::RegisterVariable(SDebugVariable Variable,TwType BarType,const char* Params)
{
	int status = 0;
	status = TwAddVarRW(m_Bars[m_CurrentBarName], Variable.name.c_str(), BarType, Variable.ptr, Params);
	assert(status);
}

void CDebugHelperImplementation::RegisterParameter(TwType ParameterType,const std::string &ParameterName, CEmptyPointerClass *Data, const std::string &Description)
{
	void *l_Data=(void *)Data;
	int status = 0;
	status = TwAddVarRW(m_Bars[m_CurrentBarName], ParameterName.c_str(), ParameterType, l_Data, Description.c_str());
	assert(status);
}

void CDebugHelperImplementation::RegisterBoolParameter(const std::string &ParameterName, CEmptyPointerClass *Data, const std::string &Description)
{
	RegisterParameter(TW_TYPE_BOOLCPP,ParameterName,Data,Description);
}

void CDebugHelperImplementation::RegisterInt32Parameter(const std::string &ParameterName, CEmptyPointerClass *Data, const std::string &Description)
{
	RegisterParameter(TW_TYPE_INT32,ParameterName,Data,Description);
}

void CDebugHelperImplementation::RegisterFloatParameter(const std::string &ParameterName, CEmptyPointerClass *Data, const std::string &Description)
{
	RegisterParameter(TW_TYPE_FLOAT,ParameterName,Data,Description);
}

void CDebugHelperImplementation::RegisterVect2fParameter(const std::string &ParameterName, CEmptyPointerClass *Data, const std::string &Description)
{
	RegisterParameter(m_Vector2f,ParameterName,Data,Description);
}

void CDebugHelperImplementation::RegisterVect3fParameter(const std::string &ParameterName, CEmptyPointerClass *Data, const std::string &Description)
{
	RegisterParameter(m_Vector3f,ParameterName,Data,Description);
}

void CDebugHelperImplementation::RegisterVect4fParameter(const std::string &ParameterName, CEmptyPointerClass *Data, const std::string &Description)
{
	RegisterParameter(m_Vector4f,ParameterName,Data,Description);
}

void CDebugHelperImplementation::RegisterColorParameter(const std::string &ParameterName, CEmptyPointerClass *Data, const std::string &Description)
{
	RegisterParameter(TW_TYPE_COLOR4F,ParameterName,Data,Description);
}

void CDebugHelperImplementation::RegisterColor32Parameter(const std::string &ParameterName, CEmptyPointerClass *Data, const std::string &Description)
{
	RegisterParameter(TW_TYPE_COLOR32,ParameterName,Data,Description);
}

void CDebugHelperImplementation::RegisterStringParameter(const std::string &ParameterName, CEmptyPointerClass *Data, const std::string &Description)
{
	RegisterParameter(TW_TYPE_STDSTRING, ParameterName, Data, Description);
}

void CDebugHelperImplementation::RegisterPositionOrientationParameter(const std::string &ParameterName, CEmptyPointerClass *Data, const std::string &Description)
{
	RegisterParameter(m_PosRotType,ParameterName,Data,Description);
}

void CDebugHelperImplementation::RegisterLUAButton(const std::string &ButtonName, const std::string &Script)
{
	m_LuaParameters.push_back(new std::string(Script));

	int l_Return = TwAddButton(m_Bars[m_CurrentBarName], ButtonName.c_str(), &CDebugHelperImplementation::CallLuaFunction,(void*)m_LuaParameters[m_LuaParameters.size() - 1],NULL);
	assert(l_Return == 1);
}

void CDebugHelperImplementation::RegisterLUAExtendedButton(const std::string &ButtonName, const std::string &Function, CEmptyPointerClass* Object, const std::string &ObjectType)
{
	m_LuaExtendedParameters.push_back(new ClientData(Function, Object, ObjectType));
	
	int l_Return = TwAddButton(m_Bars[m_CurrentBarName], ButtonName.c_str(), &CDebugHelperImplementation::CallLuaExtendedFunction, (void*)(m_LuaExtendedParameters[m_LuaExtendedParameters.size() - 1]), NULL);
	assert(l_Return == 1);
}

void CDebugHelperImplementation::RegisterLUAChangeTextureButton(const std::string &ButtonName, const std::string &Function, CMaterial* Material, CEmptyPointerClass* NewTexture, unsigned int Index)
{
	m_LuaChangeTextureParameters.push_back(new ChangeTextureClientData(Function, Material, NewTexture, Index));

	int l_Return = TwAddButton(m_Bars[m_CurrentBarName], ButtonName.c_str(), &CDebugHelperImplementation::CallLuaFunctionChangeTexture, (void*)(m_LuaChangeTextureParameters[m_LuaChangeTextureParameters.size() - 1]), NULL);
	assert(l_Return == 1);
}