class 'CActivableElement' (CLUAComponent)
function CActivableElement:__init(CLuaGameObject, InteractionRange, ScriptType, ActivationEvent)
CLUAComponent.__init(self,ScriptType)	
	self.m_LuaGameObject = CLuaGameObject
	self.m_Activable = false
	self.m_InteractionRange = InteractionRange
	self.m_ActivationEvent = ActivationEvent
	g_EventManager:Subscribe( self, "PLAYER_INTERACTS" )
end

function CActivableElement:UnsuscribeEvents()
	g_EventManager:Unsubscribe(self,"PLAYER_INTERACTS", true)
end

function CActivableElement:GetLuaGameObject() return self.m_LuaGameObject end
function CActivableElement:SetLuaGameObject(GameObject) self.m_LuaGameObject = GameObject end
--Event
function CActivableElement:PLAYER_INTERACTS()
	if(self.m_Activable and self.m_ActivationEvent~="") then
		g_EventManager:FireEvent(self.m_ActivationEvent)
	end
end

function CActivableElement:GetInteractionRange()	
	return self.m_InteractionRange
end

function CActivableElement:IsActivable() return self.m_Activable end

function CActivableElement:Update(ElapsedTime)
	self.m_Activable = false
	local l_Position = self.m_LuaGameObject:GetPosition()
	local l_PlayerPos = g_Player:GetPosition()
	if (CTTODMathUtils.PointInsideCircle(l_PlayerPos, l_Position, self.m_InteractionRange)) then
		self.m_Activable = true
	end
end