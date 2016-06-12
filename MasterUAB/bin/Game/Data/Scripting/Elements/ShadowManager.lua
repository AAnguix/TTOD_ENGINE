class 'CShadowManager' (CLUAComponent)
function CShadowManager:__init()
CLUAComponent.__init(self,"ShadowManagerScript")	
	self.m_Enable = false
	self.m_SpotLights = {}
	self.m_CurrentSpotlight = nil
end

function CShadowManager:AddSpotlight(Spotlight)
	table.insert(self.m_SpotLights,Spotlight)
	if 	self.m_CurrentSpotlight == nil then
		self.m_CurrentSpotlight = Spotlight
	end
end

function CShadowManager:Update(ElapsedTime)
	self:LookAtPlayer(g_Player:GetPosition(),ElapsedTime)	
end

function CShadowManager:LookAtPlayer(PlayerPosition, ElapsedTime)
	
	-- local l_Matrix = Mat44f()
	-- l_Matrix:SetIdentity()
	-- l_Matrix:SetFromLookAt(self.m_CurrentSpotlight:GetPosition(),PlayerPosition,self.m_CurrentSpotlight:GetUp())
	-- g_ContextManager:SetWorldMatrix(l_Matrix)
	
	-- local l_Forward = self.m_CurrentSpotlight:GetForward()
	-- local l_Position = self.m_CurrentSpotlight:GetPosition()
	-- local l_CurrentYaw = self.m_CurrentSpotlight:GetYaw()
	-- local l_Velocity = 3.0
	-- local l_Angle = CTTODMathUtils.GetAngleToFacePoint(l_Forward, l_Position, PlayerPosition)

	-- local l_NewYaw = CTTODMathUtils.CalculateNewAngle(l_Angle, l_CurrentYaw, l_Velocity, ElapsedTime)
	-- self.m_CurrentSpotlight:SetYaw(l_NewYaw)
	
	local l_LightPos = self.m_CurrentSpotlight:GetPosition()
	local l_Direction = PlayerPosition - l_LightPos
	l_Direction:Normalize(1)
	self.m_CurrentSpotlight:SetDirection(Vect3f(l_Direction.x,l_Direction.y+0.7,l_Direction.z))
	-- local l_Offset = Vect3f(3.0,1.0,0.0)
	-- self.m_CurrentSpotlight:SetPosition(PlayerPosition+l_Offset)
end

function CShadowManager:SwitchSpot(SpotlightName)
	for i=1, (#self.m_SpotLights) do
		if self.m_SpotLights[i]:GetName() == SpotlightName then
			self.m_CurrentSpotlight = self.m_SpotLights[i]
		end
	end
end

function CShadowManager:Enable()
	if self.m_Enable then
		self.m_Enable = false
	else self.m_Enable = true
	end
end

