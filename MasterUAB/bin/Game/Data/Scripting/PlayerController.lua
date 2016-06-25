l_CCVelocity = Vect3f(0.0,0.0,0.0)

function CPlayerComponent:PlayerController(ElapsedTime)

	self.m_Walk = false
	local g_Run = false
	self.m_Rotate = false
	
	self.m_Forward = false
	self.m_Backwards = false
	self.m_Right = false
	self.m_Left = false
	self.m_E = false
	
	local l_CameraController = g_CameraControllerManager:GetCurrentCameraController()
	
	if CInputManager.GetInputManager():IsActionActive("LOCK_CHARACTER") then
		if (self:IsLocked()) then		
			self:Unlock()
		else
			self:Lock()
		end
	end
	
	if (self:IsLocked() == false) then
		if CInputManager.GetInputManager():IsActionActive("PLAYER_ATTACKS") then	
			g_Player:GetAnimatorController():SetTrigger("Attack")
			-- self.m_Rotate = true
		end
		if CInputManager.GetInputManager():IsActionActive("PLAYER_BLOCKS") then	
			-- StartLevelTwo()
			--g_Player:GetAnimatorController():SetTrigger("Block")
		end
		
		if ((CInputManager.GetInputManager():IsActionActive("MOVE_FWD")) or 
		(CInputManager.GetInputManager():IsActionActive("MOVE_BACK")) or 
		(CInputManager.GetInputManager():IsActionActive("STRAFE_RIGHT")) or 
		(CInputManager.GetInputManager():IsActionActive("STRAFE_LEFT")))  then
			local l_Position = g_Player:GetRenderableObject():GetPosition()
			self.m_Walk = true
		end

		l_CCVelocity.x = 0
		l_CCVelocity.z = 0
		
		l_Speed = 3
		
		if CInputManager.GetInputManager():IsActionActive("E_PRESSED") then			
			self.m_E = true
		end
	
		if CInputManager.GetInputManager():IsActionActive("MOVE_FWD") then			
			self.m_Forward = true
			-- self:MatchPlayerYawToCameraYaw(l_CameraController,ElapsedTime,g_Forward,g_Backwards,g_Right,g_Left)				
			local v = l_CameraController:GetForward()
			l_CCVelocity = l_CCVelocity + v * l_Speed
		end
		
		if CInputManager.GetInputManager():IsActionActive("MOVE_BACK") then				
			self.m_Backwards = true
			-- self:MatchPlayerYawToCameraYaw(l_CameraController,ElapsedTime,g_Forward,g_Backwards,g_Right,g_Left)	
			local v = l_CameraController:GetForward()
			l_CCVelocity = l_CCVelocity - v * l_Speed
		end
		
		if CInputManager.GetInputManager():IsActionActive("STRAFE_RIGHT") then
			self.m_Right = true
			-- self:MatchPlayerYawToCameraYaw(l_CameraController,ElapsedTime,g_Forward,g_Backwards,g_Right,g_Left)		
			local v = l_CameraController:GetRight()
			l_CCVelocity = l_CCVelocity - v * l_Speed
		end
		
		if CInputManager.GetInputManager():IsActionActive("STRAFE_LEFT") then
			self.m_Left = true
			-- self:MatchPlayerYawToCameraYaw(l_CameraController,ElapsedTime,g_Forward,g_Backwards,g_Right,g_Left)		
			local v = l_CameraController:GetRight()
			l_CCVelocity = l_CCVelocity + v * l_Speed
		end
		
		l_CCVelocity = l_CCVelocity + Vect3f(0,-10.0,0) * ElapsedTime
		
		if g_Engine:LoadingLevel() == false then
			if ElapsedTime>0.0 then
				l_CCVelocity = g_PhysXManager:DisplacementCharacterController(g_Player:GetName(), (l_CCVelocity * ElapsedTime), ElapsedTime)
			else 
				l_CCVelocity = g_PhysXManager:DisplacementCharacterController(g_Player:GetName(), (l_CCVelocity), ElapsedTime)
			end 
			
			g_Player:GetAnimatorController():SetBool("Walk", self.m_Walk)
			g_Player:GetAnimatorController():SetBool("Rotate",self.m_Rotate)
			--GetPlayer():GetAnimatorController():SetBool("Run", g_Run); 
		end
		
	end --END CharBlock

end


function CPlayerComponent:MatchPlayerYawToCameraYaw (CameraController, ElapsedTime, Forward, Backwards, Right, Left)
	local l_Offset = 0.0
	
	if Forward then
		if Right then
			l_Offset = -0.785398
		elseif Left then
			l_Offset = 0.785398
		end
	elseif Backwards then
		if Right then
			l_Offset = 0.785398
		elseif Left then
			l_Offset = -0.785398
		end
	elseif Right then
		l_Offset = -1.5708
	elseif Left  then
		l_Offset = 1.5708	
	end
	
	
	--local l_CurrentYaw = CameraController:GetYaw()
	-- local l_Result = 0.0
	
	-- if l_Offset < 0.0 then 
		-- if (self.m_RotationVelocity*(-1)*ElapsedTime) < l_Offset then
			-- l_Result = l_CurrentYaw+(l_Offset*ElapsedTime)
		-- else
			-- l_Result = l_CurrentYaw+(ElapsedTime*self.m_RotationVelocity*(-1))
		-- end
	-- else
		-- if (self.m_RotationVelocity*ElapsedTime) > l_Offset then
			-- l_Result = l_CurrentYaw+ (l_Offset*ElapsedTime)
		-- else
			-- l_Result = l_CurrentYaw+ (ElapsedTime*self.m_RotationVelocity)
		-- end
	-- end
	
	-- g_LogManager:Log(l_Result.."")
	-- g_Player:SetYaw(l_Result)
		
	g_Player:GetRenderableObject():SetYaw(CameraController:GetYaw()+(l_Offset))
end

-- function CPlayerComponent:CalculateNewAngle(Angle, CurrentYaw, Velocity, ElapsedTime)
	-- local l_Result = 0.0
	-- if Angle < 0.0 then
		-- if (Velocity*(-1)*ElapsedTime) < Angle then
			-- l_Result = CurrentYaw + (Angle*ElapsedTime)
		-- else l_Result = CurrentYaw + (Velocity*(-1)*ElapsedTime)
		-- end
	-- else
		-- if (Velocity*ElapsedTime) > Angle then
			-- l_Result = CurrentYaw + (Angle*ElapsedTime)
		-- else l_Result = CurrentYaw + (Velocity*ElapsedTime)
		-- end
	-- end
	-- return l_Result
-- end