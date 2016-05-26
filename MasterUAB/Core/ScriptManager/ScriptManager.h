#ifndef _SCRIPTMANAGER_H
#define _SCRIPTMANAGER_H

#include <string>
#include <vector>

extern "C"
{
	#include "lua.h"
	#include "lualib.h"
	#include "lauxlib.h"
}

class CScriptManager
{
private:
	lua_State *m_LS;
	void RegisterGraphics();
	void RegisterRender();
	void RegisterAnimations();
	void RegisterCamera();
	void RegisterCinematics();
	void RegisterEffects();
	void RegisterLights();
	void RegisterMaterials();
	void RegisterParticles();
	void RegisterRenderableObjects();
	void RegisterBase();
	void RegisterCore();
	void RegisterComponents();
	void RegisterPhysics();
	void RegisterSceneRendererCommands();
	void RegisterGUI();
	void RegisterSound();
	void Register();

public:
	CScriptManager();
	virtual ~CScriptManager();
	void Initialize();
	void RunLuaMain();
	void Destroy();
	void RunCode(const std::string &Code) const;
	void RunFile(const std::string &FileName) const;
	void Load(const std::string &XMLFile);
	lua_State * GetLuaState() const {return m_LS;}
	void RegisterLUAFunctions();
	
};

#endif