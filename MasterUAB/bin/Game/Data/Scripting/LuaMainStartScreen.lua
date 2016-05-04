function LuaMain()
	DefineLUAModules()
	Initialize()
end

function DefineLUAModules()
	DefineFunctions()
end

function DefineFunctions()
	dofile("./Data/Scripting/GlobalsStartScreen.lua")
	dofile("./Data/Scripting/GUIStartScreen.lua")
	dofile("./Data/Scripting/CameraController.lua")
	dofile("./Data/Scripting/Reload.lua")
	dofile("./Data/Scripting/AntTweakBar.lua")
end

function Initialize()
	InitializeDebugBar()
	InitializeGUI()
end

function Update(ElapsedTime)
	Reload(ElapsedTime)
	CameraController(ElapsedTime)
	UpdateGUI(ElapsedTime)
end

