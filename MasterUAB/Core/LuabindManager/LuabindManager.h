#ifndef _LUABINDMANAGER_H
#define _LUABINDMANAGER_H

#include <string>
#include <vector>

extern "C"
{
	#include "lua.h"
	#include "lualib.h"
	#include "lauxlib.h"
}

class CLuabindManager
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
	void RegisterAI();
	void RegisterBase();
	void RegisterCore();
	void RegisterComponents();
	void RegisterPhysics();
	void RegisterSceneRendererCommands();
	void RegisterGUI();
	void RegisterSound();
	void Register();

public:
	CLuabindManager();
	virtual ~CLuabindManager();
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