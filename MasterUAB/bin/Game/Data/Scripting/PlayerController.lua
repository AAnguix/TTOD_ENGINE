
CharBlock = false

l_CCVelocity = Vect3f(0.0,0.0,0.0)

function CPlayerComponent:PlayerController(ElapsedTime)

	g_Walk = false
	g_Run = false
	
	l_CameraController = g_CameraControllerManager:GetCurrentCameraController()
	
	if CInputManager.GetInputManager():IsActionActive("PRUEBA") then
		
		if g_PlayerComponent:IsLocked() then		
			g_PlayerComponent:UnLock()
		else 	
			g_PlayerComponent:Lock()
		end
	end
	
	if g_PlayerComponent:IsLocked() == false then
	
		if CInputManager.GetInputManager():IsActionActive("PLAYER_ATTACKS") then	
			g_Player:GetAnimatorController():SetTrigger("Attack")
		end
		if CInputManager.GetInputManager():IsActionActive("PLAYER_BLOCKS") then	
			g_Player:GetAnimatorController():SetTrigger("Block")
			-- local l_SoundEvent = SoundEvent("Play_Hit")
			-- g_SoundManager:PlayEvent(l_SoundEvent)		
		end
	
		if ((CInputManager.GetInputManager():IsActionActive("PLAYER_WALK_FORWARD")) or 
		(CInputManager.GetInputManager():IsActionActive("PLAYER_WALK_BACK")) or 
		(CInputManager.GetInputManager():IsActionActive("PLAYER_WALK_LEFT")) or 
		(CInputManager.GetInputManager():IsActionActive("PLAYER_WALK_RIGHT")))  then
			g_Walk = true
			--l_Player:BlendCycle(2,1.0,0.1)
			--g_LogManager:Log("Entrado Aca")
		--else
			--g_Player:ClearCycle(2,0.1)
			--g_Player:BlendCycle(0,1.0,0.1)
		end
		
		l_CCVelocity.x = 0
		l_CCVelocity.z = 0
		
		l_Speed = 3
		
		if CInputManager.GetInputManager():IsActionActive("MOVE_FWD") then			
			self:MatchPlayerYawToCameraYaw(l_CameraController)				
			local v = l_CameraController:GetForward()
			l_CCVelocity = l_CCVelocity + v * l_Speed
		end
		
		if CInputManager.GetInputManager():IsActionActive("MOVE_BACK") then				
			self:MatchPlayerYawToCameraYaw(l_CameraController)
			local v = l_CameraController:GetForward()
			l_CCVelocity = l_CCVelocity - v * l_Speed
		end
		
		if CInputManager.GetInputManager():IsActionActive("STRAFE_RIGHT") then
			
			self:MatchPlayerYawToCameraYaw(l_CameraController)	
			local v = l_CameraController:GetRight()
			l_CCVelocity = l_CCVelocity - v * l_Speed
		end
		
		if CInputManager.GetInputManager():IsActionActive("STRAFE_LEFT") then
			self:MatchPlayerYawToCameraYaw(l_CameraController)	
			local v = l_CameraController:GetRight()
			l_CCVelocity = l_CCVelocity + v * l_Speed
		end
		
		if CInputManager.GetInputManager():IsActionActive("PRUEBA") then
			--Jump(true,ElapsedTime)
			l_CCVelocity.y = 3
		end
		
		l_CCVelocity = l_CCVelocity + Vect3f(0,-10.0,0) * ElapsedTime
		
		--g_PhysXManager:DisplacementCharacterController(g_Player:GetName(), GetMoveBuffer(), ElapsedTime,3)
		
		if ElapsedTime>0.0 then
			l_CCVelocity = g_PhysXManager:DisplacementCharacterController(g_Player:GetName(), (l_CCVelocity * ElapsedTime), ElapsedTime)
		else 
			l_CCVelocity = g_PhysXManager:DisplacementCharacterController(g_Player:GetName(), (l_CCVelocity), ElapsedTime)
		end 
		
		g_Player:GetAnimatorController():SetBool("Walk", g_Walk);
		g_Player:GetAnimatorController():SetBool("Run", g_Run);
		
	end --end player locked
	
end

function CPlayerComponent:MatchPlayerYawToCameraYaw (CameraController)
	g_Player:SetYaw(CameraController:GetYaw())
end

-- function Jump(Speed, ElapsedTime)

	-- local l_ConstantSpeed=ElapsedTime*g_PlayerSpeed
	
	-- if Speed then
		-- l_ConstantSpeed=l_ConstantSpeed*g_PlayerFastSpeed
	-- end

	-- local Displacement = Vect3f(0.0,12.0,5.0)
	
	-- if Displacement:SquaredLength() > 0 then
		 -- Displacement:Normalize(1)
		 -- Displacement = Displacement*l_ConstantSpeed
		-- l_Position = g_PhysXManager:MoveCharacterController(g_Player:GetName(),Displacement,ElapsedTime)
		-- g_Player:SetPosition(l_Position)
	-- end
-- end