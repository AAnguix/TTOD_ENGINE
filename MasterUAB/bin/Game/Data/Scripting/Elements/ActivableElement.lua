class 'CActivableElement' (CLUAComponent)
function CActivableElement:__init(CLuaGameObject, InteractionRange, ScriptType, ActivationEvent)
CLUAComponent.__init(self,ScriptType)	
	self.m_LuaGameObject = CLuaGameObject
	self.m_Activable = false
	self.m_InteractionRange = InteractionRange
	self.m_InteractionAngle = 1.5707
	self.m_ActivationEvent = ActivationEvent
	g_EventManager:Subscribe( self, "PLAYER_INTERACTS" )
	g_SoundManager:AddComponent(self.m_LuaGameObject:GetName().."_AudioSource", self.m_LuaGameObject)
	self.m_LuaGameObject:AddSound("PlayerInteraction2","Play_Interaction2")
end

function CActivableElement:UnsuscribeEvents()
	g_EventManager:Unsubscribe(self,"PLAYER_INTERACTS", true)
end

function CActivableElement:GetLuaGameObject() return self.m_LuaGameObject end
function CActivableElement:SetLuaGameObject(GameObject) self.m_LuaGameObject = GameObject end
--Eventb
function CActivableElement:PLAYER_INTERACTS() 
	if(self.m_Activable and self.m_ActivationEvent~="") then
		if (not g_PlayerComponent:IsLocked()) then 
			g_PlayerComponent:GetLuaGameObject():SetTrigger("Interact")
			local l_PlayerLuaGameObject = g_PlayerComponent:GetLuaGameObject()
			local l_DirectionToFace = (self.m_LuaGameObject:GetPosition() - l_PlayerLuaGameObject:GetPosition()):Normalize(1)
			local l_ForwardBeforeFacing = l_PlayerLuaGameObject:GetForward():Normalize(1)
			
			g_LogManager:Log("Player")
			g_LogManager:Log(l_ForwardBeforeFacing)
			g_LogManager:Log("Pedestal")
			g_LogManager:Log(l_DirectionToFace)
			
			g_PlayerComponent:SetInteractionFacingValues(l_ForwardBeforeFacing,l_DirectionToFace)
			g_EventManager:FireEvent(self.m_ActivationEvent)
		end
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
	local l_PlayerForward = g_Player:GetForward()
	local l_Angle = CTTODMathUtils.GetAngleToFacePoint(l_PlayerForward,l_PlayerPos,l_Position)
	
	if ((CTTODMathUtils.PointInsideCircle(l_PlayerPos, l_Position, self.m_InteractionRange)) and (l_Angle<self.m_InteractionAngle and l_Angle>(self.m_InteractionAngle*-1.0))) then
		self.m_Activable = true
	end
end