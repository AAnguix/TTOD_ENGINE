class 'CStoneGatePedestalComponent' (CActivableElement)
function CStoneGatePedestalComponent:__init(CLuaGameObject, Event, StoneGateLuaGameObject)
	CActivableElement.__init(self, CLuaGameObject, 1.2, "StoneDoorPedestal",Event)
	self.m_Event = Event
	self.m_Disabled = false
	self.m_StoneGateLuaGameObject = StoneGateLuaGameObject
	
	g_SoundManager:AddComponent(self.m_LuaGameObject:GetName().."_AudioSource", self.m_LuaGameObject)
	self.m_LuaGameObject:AddSound("StoneGateMovement","Play_StoneGateSound")
	
	local l_StoneGatePosition = self.m_StoneGateLuaGameObject:GetPosition()
	g_PhysXManager:CreateBoxTrigger(self.m_StoneGateLuaGameObject:GetName(),Vect3f(1.0,3.0,1.0),"",l_StoneGatePosition,Quatf(0.0,0.0,0.0,1.0),"kinematic")
end

function CStoneGatePedestalComponent:STONE_GATE_PEDESTAL_ACTIVATED()
	self.m_LuaGameObject:PlayEvent("StoneGateMovement")
	self:Disable()
	local l_Displacement = Vect3f(2.0,0.0,0.0)
	self.m_StoneGateLuaGameObject:SetPosition(self.m_StoneGateLuaGameObject:GetPosition()+l_Displacement)
end

function CStoneGatePedestalComponent:Update(ElapsedTime)
	if (not self.m_Disabled) then
		CActivableElement.Update(self,ElapsedTime)
		if CActivableElement.IsActivable(self) then
			local l_Pos = SGUIPosition(0.4,0.7,0.1,0.1,CGUIManager.TOP_CENTER,CGUIManager.GUI_RELATIVE,CGUIManager.GUI_RELATIVE_WIDTH)
			g_GUIManager:DoText("StoneDoorPedestal_Text","freestyle_script_48_font",l_Pos,"","[E] Activate")
		end
	end
end

function CStoneGatePedestalComponent:Disable() self.m_Disabled = true end
function CStoneGatePedestalComponent:Enable() self.m_Disabled = false end