function CPlayerComponent:WalkController()
	if(self.m_Forward == (not self.m_Backwards))then
		self.m_Walk = true
	elseif(self.m_Right == (not self.m_Left))then
		self.m_Walk = true
	else self.m_Walk = false
	end
	g_Player:SetBool("Walk", self.m_Walk)
end

-- function CPlayerComponent:MovementEquation(Acceleration, ElapsedTime)
	-- local l_SquaredElapsedTime = ElapsedTime*ElapsedTime
	-- return ((Acceleration*0.5)*(l_SquaredElapsedTime))
-- end

function CPlayerComponent:PlayerController(ElapsedTime)
	
	if self.m_BeeingTossed then
		self.m_Velocity = self.m_TossedDirection
	end
	self:AddGravity()
	
	local l_Pos = SGUIPosition(0.4,0.4,0.2,0.2,CGUIManager.TOP_CENTER,CGUIManager.GUI_RELATIVE,CGUIManager.GUI_RELATIVE_WIDTH)
	g_GUIManager:DoText("PlayerVelocityText","calibri_font",l_Pos,"","Vel. x: "..Round(self.m_Velocity.x,2)..", y: "..Round(self.m_Velocity.y,2)..", z: "..Round(self.m_Velocity.z,2))
	
	if g_Engine:LoadingLevel() == false then
		if ElapsedTime>0.0 then
			self.m_Velocity = g_PhysXManager:DisplacementCharacterController(g_Player:GetName(), (self.m_Velocity * ElapsedTime), ElapsedTime)
		else 
			self.m_Velocity = g_PhysXManager:DisplacementCharacterController(g_Player:GetName(), (self.m_Velocity), ElapsedTime)
		end 
		self:WalkController()
	end
	
	self.m_Velocity.x = 0
	self.m_Velocity.y = 0
	self.m_Velocity.z = 0
	
	self.m_Walk = false
	self.m_Forward = false
	self.m_Backwards = false
	self.m_Right = false
	self.m_Left = false
end

-- function CPlayerComponent:MatchPlayerYawToCameraYaw (CameraController, ElapsedTime, Forward, Backwards, Right, Left)
	-- local l_Offset = 0.0
	
	-- if Forward then
		-- if Right then
			-- l_Offset = -0.785398
		-- elseif Left then
			-- l_Offset = 0.785398
		-- end
	-- elseif Backwards then
		-- if Right then
			-- l_Offset = 0.785398
		-- elseif Left then
			-- l_Offset = -0.785398
		-- end
	-- elseif Right then
		-- l_Offset = -1.5708
	-- elseif Left  then
		-- l_Offset = 1.5708	
	-- end
		
	-- g_Player:SetYaw(CameraController:GetYaw()+(l_Offset))
-- end