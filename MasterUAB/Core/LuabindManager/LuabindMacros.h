#ifndef _LUABINDMACROS_H
#define _LUABINDMACROS_H

#define REGISTER_LUA_FUNCTION(FunctionName,AddrFunction) {luabind::module(LUA_STATE) [ luabind::def(FunctionName,AddrFunction) ];}
#define LUA_STATE CEngine::GetSingleton().GetLuabindManager()->GetLuaState()

#endif