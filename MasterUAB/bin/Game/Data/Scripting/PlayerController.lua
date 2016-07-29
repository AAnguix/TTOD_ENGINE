function CPlayerComponent:PlayerController(ElapsedTime)
	
	if (self:IsLocked() == false) then
		
		self:AddGravity(ElapsedTime)
		
		local l_Pos = SGUIPosition(0.4,0.4,0.2,0.2,CGUIManager.TOP_CENTER,CGUIManager.GUI_RELATIVE,CGUIManager.GUI_RELATIVE_WIDTH)
		g_GUIManager:DoText("aa","calibri_font",l_Pos,"","Velocity: "..self.m_Velocity.x..","..self.m_Velocity.y..","..self.m_Velocity.z)
			
		if g_Engine:LoadingLevel() == false then
			if ElapsedTime>0.0 then
				self.m_Velocity = g_PhysXManager:DisplacementCharacterController(g_Player:GetName(), (self.m_Velocity * ElapsedTime), ElapsedTime)
			else 
				self.m_Velocity = g_PhysXManager:DisplacementCharacterController(g_Player:GetName(), (self.m_Velocity), ElapsedTime)
			end 
			g_Player:SetBool("Walk", self.m_Walk)
		end
		
		self.m_Velocity.x = 0
		self.m_Velocity.z = 0
		
		self.m_Walk = false
		self.m_Forward = false
		self.m_Backwards = false
		self.m_Right = false
		self.m_Left = false
		
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
		
	g_Player:SetYaw(CameraController:GetYaw()+(l_Offset))
end