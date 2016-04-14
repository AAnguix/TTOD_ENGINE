#include "ScriptManager.h"
#include <assert.h>
#include <luabind/luabind.hpp>

#include <luabind/operator.hpp>
#include <luabind/iterator_policy.hpp>

#include "Engine.h"
#include "SceneRendererCommands\SceneRendererCommandManager.h"
#include "Log.h"

using namespace luabind;

#define LUA_STATE CEngine::GetSingleton().GetScriptManager()->GetLuaState()
#define REGISTER_LUA_FUNCTION(FunctionName,AddrFunction) {luabind::module(LUA_STATE) [ luabind::def(FunctionName,AddrFunction) ];}

CScriptManager::CScriptManager()
: m_LS(NULL)
{
}

CScriptManager::~CScriptManager()
{
	Destroy();	
}

int Alert(lua_State * State) 
{  
	std::string l_Text;  
	int n = lua_gettop(State);  
	int i;  lua_getglobal(State, "tostring");  
	for (i=1; i<=n; i++) 
	{   const char *s;   
	lua_pushvalue(State, -1);   
	lua_pushvalue(State, i);   
	lua_call(State, 1, 1); 
  
	 s = lua_tostring(State, -1);   
	 if (s == NULL)     
		return luaL_error(State, "`tostring' must return a string to `print'");   
	 
	 if (i>1) l_Text += '\t';   
	 l_Text += s;   
	 
	 lua_pop(State, 1);   
}  

l_Text += '\n';  

CEngine::GetSingleton().GetLogManager()->Log(l_Text.c_str());
//Info( l_Text.c_str() );  
return true; 

}

int ShowLuaErrorDebugInfo(lua_State* L)
{
	lua_Debug d;
	lua_getstack(L, 1, &d);
	lua_getinfo(L, "Sln", &d);
	std::string err = lua_tostring(L, -1);
	lua_pop(L, 1);
	std::stringstream msg;
	msg << d.short_src << ":" << d.currentline;

	if (d.name != 0)
	{
	msg << "(" << d.namewhat << " " << d.name << ")";
	}
	msg << " " << err;
	CEngine::GetSingleton().GetLogManager()->Log(msg.str().c_str());
	return 0;
}

void CScriptManager::Initialize() 
{  
	m_LS=lua_open();
	luaopen_base(m_LS);
	luaopen_string(m_LS);
	luaopen_table(m_LS);
	luaopen_math(m_LS);
	luaopen_debug(m_LS);
	//Sobreescribimos la función _ALERT de LUA cuando se genere algún error al ejecutar código LUA 
	lua_register(m_LS,"_ALERT",Alert);

	//Initialize luabind
	luabind::open(m_LS);

	lua_atpanic(m_LS, ShowLuaErrorDebugInfo);
	luabind::set_pcall_callback(ShowLuaErrorDebugInfo);

	RegisterLUAFunctions();
	RunFile("./Data/scripting/LuaMainStartScreen.lua");
	RunCode("LuaMain()");
} 

void CScriptManager::Destroy() 
{  
	if(m_LS!=NULL)
	lua_close(m_LS); 
}  

//Execute code fragment in LUA
void CScriptManager::RunCode(const std::string &Code) const 
{  
	if(luaL_dostring(m_LS,Code.c_str()))  
	{   
		const char *l_Str=lua_tostring(m_LS, -1);   
		//Info("%s",l_Str);
		CEngine::GetSingleton().GetLogManager()->Log(l_Str);
		//assert(!"must be log");
	} 
}  

//Execute LUA file
void CScriptManager::RunFile(const std::string &FileName) const 
{  
	if(luaL_dofile(m_LS, FileName.c_str()))  
	{   const char *l_Str=lua_tostring(m_LS, -1);   
		CEngine::GetSingleton().GetLogManager()->Log(l_Str);
		//Info("%s",l_Str); 
		//assert(!"must be log");
	} 
}   

void CScriptManager::RegisterLUAFunctions()
{
	RegisterBase();
	RegisterSound();
	RegisterCore();
	RegisterGraphics();
	RegisterGUI();
	RegisterPhysics();
}

void CScriptManager::RegisterSceneRendererCommands()
{
	module(LUA_STATE) 
	[  
		class_<CTemplatedVectorMapManager<CSceneRendererCommand>>("CTemplatedVectorMapManager")   
		.def("GetResource", &CTemplatedVectorMapManager<CSceneRendererCommand>::GetResource) 
		.def("GetResourceById", &CTemplatedVectorMapManager<CSceneRendererCommand>::GetResourceById) 
		.def("AddResource", &CTemplatedVectorMapManager<CSceneRendererCommand>::AddResource) 
		.def("RemoveResource", &CTemplatedVectorMapManager<CSceneRendererCommand>::RemoveResource) 
		.def("GetResourcesMap", &CTemplatedVectorMapManager<CSceneRendererCommand>::GetResourcesMap) 
		.def("GetResourcesVector", &CTemplatedVectorMapManager<CSceneRendererCommand>::GetResourcesVector) 
	]; 

	module(LUA_STATE) 
	[ 
		class_< CSceneRendererCommandManager, CTemplatedVectorMapManager<CSceneRendererCommand>>("CSceneRendererCommandManager")   
		.def("Reload", &CSceneRendererCommandManager::Reload) 
		.def("Load", &CSceneRendererCommandManager::Load) 
		.def("Execute", &CSceneRendererCommandManager::Execute) 
	];

	module(LUA_STATE) 
	[
		class_<CActive>("CActive")
		.def(constructor<CXMLTreeNode>())
		.def("SetActive", &CActive::SetActive)
		.def("GetActive", &CActive::GetActive)
	];

	module(LUA_STATE) 
	[
		class_<CSceneRendererCommand,bases<CActive,CNamed>>("CSceneRendererCommand")
		.def("execute", &CSceneRendererCommand::Execute)
	];
}

void CScriptManager::Load(const std::string &XMLFile)
{

}