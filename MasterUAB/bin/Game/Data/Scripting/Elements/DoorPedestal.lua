class 'CDoorPedestalComponent' (CPedestalComponent)
function CDoorPedestalComponent:__init(CGameObject)
CPedestalComponent.__init(CGameObject)	
end

function CDoorPedestalComponent:Update(ElapsedTime)
	if self.m_Active == true then
		self:AddTime(ElapsedTime)
		if(self:GetTimer() <= self.m_TimeActive) then
			-- if self.m_SoundPlayed == false then
				-- self.m_GameObject:GetAudioSource():PlayEvent("pedestal_sound")
				-- self.m_SoundPlayed = true
			-- end
			--Show GUI message telling player that he has reach and objetive
		else
			self.m_Active = false
			self:ResetTimer()
			self:Disable()
		end
	elseif self:IsPlayerInsideInteractionRange(g_Player:GetRenderableObject():GetPosition()) then
		self:ShowGuiMessage()
	end
end

function CDoorPedestalComponent:ShowGuiMessage()
	
	local l_Pos = SGUIPosition(0.4,0.7,0.1,0.1,CGUIManager.TOP_CENTER,CGUIManager.GUI_RELATIVE,CGUIManager.GUI_RELATIVE_WIDTH)
	g_GUIManager:DoText("DorrPedestalText","felix_font",l_Pos,"","Press E to activate")
	
	if g_PlayerComponent.m_E then
		self.m_Active = true
	end
end