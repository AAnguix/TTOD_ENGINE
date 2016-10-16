class 'CTriggerComponent' (CLUAComponent)
function CTriggerComponent:__init(CLuaGameObject, Event)
CLUAComponent.__init(self,"Trigger")	
	self.m_LuaGameObject = CLuaGameObject
	self.m_Event = Event
end

function CTriggerComponent:OnTriggerEnter(Actor)
	if(Actor=="Player") then
		g_EventManager:FireEvent(self.m_Event)
	end
end


