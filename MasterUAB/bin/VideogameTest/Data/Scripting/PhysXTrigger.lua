-- function OnCollidefountain(Actor)
	-- if (Actor == "bruja")then	
		-- g_WindMildEnter = true
	-- end
-- end

function OnCollideTrigger_Box(Actor)
	if (Actor == "bruja")then	
		g_WindMildEnter = true
		g_PlayerLUA:Damage(20.0)
	end
end

function OnCollideTrigger_Sphere(Actor)
	if (Actor == "bruja")then	
		g_WindMildEnter = false
	end
end



