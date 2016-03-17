function LuaMain()
	local l_Engine = CEngine.get_singleton()
	l_Engine:get_script_manager():run_file("./data/scripting/CharacterController.lua")
	l_Engine:get_script_manager():run_file("./data/scripting/Reload.lua")
	l_Engine:get_script_manager():run_file("./data/scripting/AntTweakBar.lua")
end