class 'CStatueComponent' (CActivableElement)
function CStatueComponent:__init(CLuaGameObject, GrowthTime)
	CActivableElement.__init(self, CLuaGameObject, 20.0, "Statue","THROW_MAGIC_BALL")   --1.4
	self.m_Enabled = false
	self.m_MagicBallThrown = false
	self.m_GrowthTime = GrowthTime
	
	local l_AudioSourceName = self.m_LuaGameObject:GetName().."_AudioSource"
	g_SoundManager:AddComponent(l_AudioSourceName, self.m_LuaGameObject)
	--self.m_LuaGameObject:AddSound("MagicBallSound","Play_MagicBallSound")
	
	g_EventManager:Subscribe( self, "PLAYER_INTERACTS" )
end

function CStatueComponent:Update(ElapsedTime)
	if (self.m_Enabled) then
		self:AddTime(ElapsedTime)
		CActivableElement.Update(self,ElapsedTime)
		if CActivableElement.IsActivable(self) then
			local l_Pos = SGUIPosition(0.4,0.7,0.1,0.1,CGUIManager.TOP_CENTER,CGUIManager.GUI_RELATIVE,CGUIManager.GUI_RELATIVE_WIDTH)
			g_GUIManager:DoText("MagicBallText","felix_font",l_Pos,"","Cast Magic Ball [E]")
		end
	
	end
end

function CStatueComponent:Enable(Value)	self.m_Enabled = Value end

function CStatueComponent:GetGrowthTime() return self.m_GrowthTime end