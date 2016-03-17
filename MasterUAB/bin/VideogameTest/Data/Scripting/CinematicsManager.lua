function CinematicsManager()
	g_CinematicActionManager = CCinematicsActionManager()
	g_CinematicActionManager:LoadXML("./Data/Scripting/Cinematics_actions.xml")
end

function UpdateCinematics(ElapsedTime)
	
	if g_WindMildEnter == true then
		g_CinematicActionManager:Update(ElapsedTime)
	end
end