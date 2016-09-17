class 'CStoneGateComponent' (CLUAComponent)
function CStoneGateComponent:__init(CLuaGameObject)
CLUAComponent.__init(self,"StoneGate")	
	self.m_LuaGameObject = CLuaGameObject
end

function CStoneGateComponent:OnTriggerEnter()
	-- Cargar siguiente Sala
	StartBossLevel()
end


