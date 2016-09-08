class 'CStatuesManagerComponent' (CLUAComponent)
function CStatuesManagerComponent:__init(CLuaGameObject)
	CLUAComponent.__init(self,"StatuesManager")	
	self.m_Statues = {}
	self.m_CurrentStatue = nil
	self.m_CurrentStatueIndex = 1
	self.m_MagicBallSpawnDelay = 5.0
	self.m_MagicBallPositionOffset = Vect3f(0.0,2.5,0.0)
	self.m_SpawnMagicBall = true
	self.m_GrowMagicBall = false
	self.m_ScaleFactor = 0.6
	self.m_LuaGameObject = CLuaGameObject
	g_EventManager:Subscribe( self, "DRAGON_IMPACTED_BY_MAGIC_BALL" )
end

function CStatuesManagerComponent:AddStatue(StatueLuaGameObject, GrowTime)
	local l_Statue = CStatueComponent(StatueLuaGameObject, GrowTime)
	g_ScriptManager:AddComponent(StatueLuaGameObject:GetName(),StatueLuaGameObject,l_Statue)
	table.insert(self.m_Statues,l_Statue)
	
	if (self.m_CurrentStatue == nil) then
		self:SetCurrentStatue(1)
	end
end

function CStatuesManagerComponent:SetCurrentStatue(StatueIndex)
	
	if (self.m_CurrentStatue~=nil) then self.m_CurrentStatue:Enable(false) end
	
	if(StatueIndex<=3) then
		self.m_CurrentStatue = self.m_Statues[StatueIndex]
		g_LogManager:Log("setting statue "..StatueIndex)
	end
end

function CStatuesManagerComponent:Update(ElapsedTime)
	self:AddTime(ElapsedTime)
	if self.m_SpawnMagicBall then
		if self:GetTimer() >= self.m_MagicBallSpawnDelay then
			self.m_SpawnMagicBall = false
			self:ResetTimer()
			self.m_GrowMagicBall = true
			local l_MagicBallPosition = self.m_CurrentStatue:GetLuaGameObject():GetPosition() + self.m_MagicBallPositionOffset
			g_EventManager:FireEvent("START_GROWING_MAGIC_BALL", l_MagicBallPosition)
		end
	elseif self.m_GrowMagicBall then
		local l_GrowTime = self.m_CurrentStatue:GetGrowthTime()
		local l_Scale = (self:GetTimer()/l_GrowTime)*self.m_ScaleFactor
		if l_Scale>1.0 then l_Scale = 1.0 end
		if l_Scale<=0.0 then l_Scale = 0.001 end
		g_MaterialManager:GetResource("FireBall"):SetFloatParameterValue("scale",l_Scale)
		if self:GetTimer() >= l_GrowTime then
			self:ResetTimer()
			self.m_GrowMagicBall = false
			g_EventManager:FireEvent("STOP_GROWING_MAGIC_BALL")
			self.m_CurrentStatue:Enable(true)
			--Play Stop Growing Sound
		end
	end
end

function CStatuesManagerComponent:DRAGON_IMPACTED_BY_MAGIC_BALL()
	g_LogManager:Log("player impacted")
	self.m_CurrentStatueIndex = self.m_CurrentStatueIndex + 1
	g_LogManager:Log("index "..self.m_CurrentStatueIndex)
	self:SetCurrentStatue(self.m_CurrentStatueIndex)
	self.m_SpawnMagicBall = true
	--Play Dragon Impacted Sound
end

function CShowHealthBarManagerComponent:GetLuaGameObject() return self.m_LuaGameObject end