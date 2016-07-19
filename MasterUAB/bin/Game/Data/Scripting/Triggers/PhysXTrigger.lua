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

function OnTriggerEnterCristalmanec(xx)

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

function OnTriggerEntercm2(Subject)
	
	if Subject == "Enemy1" then
		--g_LogManager:Log("Aqui entramos En el trigger a pegarle al enemigo")
		--local l_Object = g_GameController:GetEnemy(Subject)
		--l_Object:TakeDamage(20.0)
	end
	
	if Subject == "Player" then
		local l_Parent = g_GameController:GetEnemy('Enemy1')--SI pasaba el enemy con "" no agarraba
		local l_AttackingState = l_Parent:IsAttacking()
		
		if l_AttackingState == true then
			
			if g_PlayerComponent:IsBlocking() == true then
				local l_MinBlock = 1.74
				local l_MaxBlock = 4.53
				local l_Angle = CTTODMathUtils.AngleBetweenVectors(g_Player:GetRenderableObject():GetForward(),l_Parent:GetRenderableObject():GetForward())
				math.abs(l_Angle)
				g_LogManager:Log(l_Angle.." Angulos")
				if (l_Angle > l_MinBlock and l_Angle < l_MaxBlock) then
					g_LogManager:Log("Ataque bloqueado")
					l_Parent:SetAttacking(false)  --Para que no entre mas de 1 vez por ataque, Verificar cunado las fisicas se muevan bien
				else
					g_LogManager:Log("Bloqueo Levantado pero el ataque Pego")
					g_PlayerComponent:TakeDamage(20.0)
					l_Parent:SetAttacking(false)  --Para que no entre mas de 1 vez por ataque, Verificar cunado las fisicas se muevan bien
				end
			else
				g_LogManager:Log("Choque Trigger y player con el flag Attackng levantado")
				g_PlayerComponent:TakeDamage(20.0)
				l_Parent:SetAttacking(false)  --Para que no entre mas de 1 vez por ataque, Verificar cunado las fisicas se muevan bien
			end
			
		else
			g_LogManager:Log("Choca trigger y player pero no se estan atacando")
		end
	end	
	
end

function OnTriggerEntercm(Subject)
	
	--g_LogManager:Log(Subject.." El arma del Player Le Pegando A esto")
	if Subject == "Enemy1" then
		local l_AttackingState = g_PlayerComponent:IsAttacking()
		
		if l_AttackingState== true then
			g_LogManager:Log("Le Estamos Pegando Al Enemy")
			local l_Enemy = g_GameController:GetEnemy(Subject)
			l_Enemy:TakeDamage(20.0)
			l_Enemy:SetHitState(true)
		end
	end
	
end

function OnTriggerEnterArmaEnemigo(Subject)
	
	if Subject == "Player" then
		g_LogManager:Log("Aqui entramos En el trigger")
		g_Player:TakeDamage(20.0)
	end	
end

function OnTriggerEnterArmaPlayer(Subject)
	
		local l_Object = g_GameController:GetEnemy(Subject)
		l_Object:TakeDamage(20.0)
end

function OnTriggerExitcm2(Subject)
end

function OnTriggerExitcm(Subject)
end