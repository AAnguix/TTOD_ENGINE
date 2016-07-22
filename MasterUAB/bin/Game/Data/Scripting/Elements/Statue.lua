class 'CStatueComponent' (CActivableElement)
function CStatueComponent:__init(CLuaGameObject, MagicBallLuaGameObject)
	CActivableElement.__init(self, CLuaGameObject, 5.0, "Statue")
	self.m_MagicBall = MagicBallLuaGameObject
	self.m_MagicBallCasted = false
	self.m_ShootVelocity = 2.0
	g_EventManager:Subscribe( self, "EPressed" )
	g_EventManager:Subscribe( self, "DragonImpactedByMagicBall" )
	local l_GameObject = self.m_LuaGameObject:GetGameObject()
	local l_AudioSourceName = self.m_LuaGameObject:GetName().."_AudioSource"
	g_SoundManager:AddComponent(l_AudioSourceName, l_GameObject)
	--self.m_LuaGameObject:AddSound("MagicBallSound","Play_MagicBallSound")
end

--Event
function CStatueComponent:EPressed()
	if(CActivableElement.IsActivable(self)) then
		self:CastMagicBall()
	end
end

function CStatueComponent:Update(ElapsedTime)
	self:AddTime(ElapsedTime)
	CActivableElement.Update(self,ElapsedTime)
	if CActivableElement.IsActivable(self) then
		local l_Pos = SGUIPosition(0.4,0.7,0.1,0.1,CGUIManager.TOP_CENTER,CGUIManager.GUI_RELATIVE,CGUIManager.GUI_RELATIVE_WIDTH)
		g_GUIManager:DoText("MagicBallText","felix_font",l_Pos,"","Cast Magic Ball [E]")
	end
	if self.m_MagicBallCasted then
		self:UpdateMagicBall(ElapsedTime)
	end
end

function CStatueComponent:UpdateMagicBall(ElapsedTime)
	--local l_Displacement = g_Dragon:GetPosition() - self.m_MagicBall:GetPosition()
	local l_Displacement = g_Player:GetPosition() - self.m_MagicBall:GetPosition()
	l_Displacement:Normalize(1)
	local l_Constant = 0.3
	local l_Base = 1.1
	local l_Exponent = math.sqrt(self:GetTimer())
	local l_NewPosition = self.m_MagicBall:GetPosition() + ExponentialDisplacement(l_Displacement,l_Constant,l_Base,l_Exponent)
	self.m_MagicBall:SetPosition(l_NewPosition)
end

function CStatueComponent:DragonImpactedByMagicBall()
	self.m_MagicBallCasted = false
end

function CStatueComponent:CastMagicBall()
	self.m_MagicBallCasted = true
	--g_EventManager:FireEvent("MagicBallCasted")
end