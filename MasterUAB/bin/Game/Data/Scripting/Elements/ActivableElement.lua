class 'CActivableElement' (CLUAComponent)
function CActivableElement:__init(CLuaGameObject, InteractionRange, ScriptType)
CLUAComponent.__init(self,ScriptType)	
	self.m_LuaGameObject = CLuaGameObject
	self.m_Activable = false
	self.m_InteractionRange = InteractionRange
end

function CActivableElement:GetLuaGameObject() return self.m_LuaGameObject end

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