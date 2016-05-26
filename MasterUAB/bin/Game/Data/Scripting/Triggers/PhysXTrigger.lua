-- These functions manage the Physyx events (collision, triggerEnter, wake, sleep, contact)

-- function OnCollidefountain(Actor)
	-- if (Actor == "bruja")then	
		-- g_WindMildEnter = true
	-- end
-- end

g_MusicOne = true

function OnCollideTrigger_Box(Actor)
	-- if (Actor == "bruja")then	
		-- g_WindMildEnter = true
		-- g_PlayerLUA:Damage(20.0)
	-- end
end

function OnCollideTrigger_Sphere(Actor)
	-- if (Actor == "bruja")then	
		-- g_WindMildEnter = false
	-- end
end

function OnCollideEnemy1_collision_sphere()
	
end

function OnCollideTrigger1(Player)
	if Player == "Player" then
		if g_MusicOne == true then
			local l_PlaySoundEvent = SoundEvent("Play_Unchallenged")
			local l_StopSoundEvent = SoundEvent("Stop_WolfBlood")
			g_SoundManager:PlayEvent(l_PlaySoundEvent)	
			g_SoundManager:PlayEvent(l_StopSoundEvent)
		else 
			local l_PlaySoundEvent = SoundEvent("Play_WolfBlood")
			local l_StopSoundEvent = SoundEvent("Stop_Unchallenged")
			g_SoundManager:PlayEvent(l_PlaySoundEvent)	
			g_SoundManager:PlayEvent(l_StopSoundEvent)
		end
		
		if g_MusicOne then
			g_MusicOne = false
		else g_MusicOne = true
		end
	end
end
function OnWake(Actor)
	
end

function OnSleep(Actor)
	
end

function OnContact(Actor)
	
end


