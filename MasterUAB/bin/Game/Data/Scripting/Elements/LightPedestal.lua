class 'CLightPedestalComponent' (CActivableElement)
function CLightPedestalComponent:__init(CLuaGameObject, Event, RuneLuaGameObject)
	CActivableElement.__init(self, CLuaGameObject, 1.0, "LightPedestal",Event)   --1.4
	self.m_Event = Event
	self.m_RuneLuaGameObject = RuneLuaGameObject
	self.TemporalActivationTime = 1.5
	
	self.m_RuneTemporarilyActivated = false
	
	self.m_Disabled = false
	self.m_RuneLuaGameObject:EnableRenderableObject(false)
end

function CLightPedestalComponent:ActivateRune()
	self.m_RuneLuaGameObject:EnableRenderableObject(true)
	self.m_LuaGameObject:PlayEvent("PlayerInteraction",self.m_LuaGameObject:GetName())
end

function CLightPedestalComponent:ActivateRuneTemporarily()
	self.m_RuneTemporarilyActivated = true
	self.m_RuneLuaGameObject:EnableRenderableObject(true)
end

function CLightPedestalComponent:Update(ElapsedTime)
	
	if (not self.m_Disabled) then
		CActivableElement.Update(self,ElapsedTime)
		if CActivableElement.IsActivable(self) then
			local l_Pos = SGUIPosition(0.4,0.7,0.1,0.1,CGUIManager.TOP_CENTER,CGUIManager.GUI_RELATIVE,CGUIManager.GUI_RELATIVE_WIDTH)
			g_GUIManager:DoText("LightPedestal_"..self.m_Event.."_Text","freestyle_script_48_font",l_Pos,"","[E] Activate Rune")
		end
		
		if self.m_RuneTemporarilyActivated then
			self:AddTime(ElapsedTime)
			if(self:GetTimer()>self.TemporalActivationTime) then
				self.m_RuneTemporarilyActivated = false
				self.m_RuneLuaGameObject:EnableRenderableObject(false)
				self:ResetTimer()
				g_LightPedestalsManager:HideAllRunes()
				g_LogManager:Log("Activacion temporal finalizada")
			end
		end
	end
end

function CLightPedestalComponent:Disable() self.m_Disabled = true end
function CLightPedestalComponent:Enable() self.m_Disabled = false end

function CLightPedestalComponent:GetRuneLuaGameObject()
	return self.m_RuneLuaGameObject
end