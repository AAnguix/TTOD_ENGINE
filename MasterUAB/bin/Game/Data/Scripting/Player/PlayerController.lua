function CPlayerComponent:WalkController()
	if(self.m_Forward == (not self.m_Backwards))then
		self.m_Walk = true
	elseif(self.m_Right == (not self.m_Left))then
		self.m_Walk = true
	else self.m_Walk = false
	end
	g_Player:SetBool("Walk", self.m_Walk)
end

function CPlayerComponent:FaceDirectionWhileWalking(Forward, NewDirection, ElapsedTime)
	local l_Angle = CTTODMathUtils.AngleBetweenVectors(NewDirection, Forward)  
	local l_CurrentYaw = self.m_LuaGameObject:GetYaw()
	if (math.abs(l_Angle)>self.m_AngleMargin) then
		self.m_LuaGameObject:SetYaw(CTTODMathUtils.CalculateNewAngle(l_Angle, l_CurrentYaw, self.m_RotationVelocity, ElapsedTime))
	end
end

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