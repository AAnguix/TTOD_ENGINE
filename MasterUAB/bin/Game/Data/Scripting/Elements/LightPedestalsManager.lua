class 'CLightPedestalsManagerComponent' (CLUAComponent)
function CLightPedestalsManagerComponent:__init(CLuaGameObject)
CLUAComponent.__init(self,"LightManagerScript")	
	self.m_LuaGameObject = CLuaGameObject
	self.m_LightsEnabled = false
	self.m_Lights = {}
	self.m_LightPedestals = {}
	self.m_Time = 0.0
	self.m_CombinationDone = false 
	self.m_CombinationDoneSoundDelay = 1.0
	self.m_CombinationDoneSoundPlayed = false
	
	g_EventManager:Subscribe(self, "LIGHT_PEDESTAL_ONE_ACTIVATED")
	g_EventManager:Subscribe(self, "LIGHT_PEDESTAL_TWO_ACTIVATED")
	g_EventManager:Subscribe(self, "LIGHT_PEDESTAL_THREE_ACTIVATED")
	g_EventManager:Subscribe(self, "PEDESTALS_COMBINATION_DONE")
	self.m_Orders = {3,1,2}
	self.m_Actives = {false,false,false}
	
	g_SoundManager:AddComponent(self.m_LuaGameObject:GetName().."_AudioSource", self.m_LuaGameObject)
	self.m_LuaGameObject:AddSound("PlayerInteraction3","Play_Interaction3")
	--self.m_LuaGameObject:AddSound("LightsOn","Play_LightsOn")
	--self.m_LuaGameObject:AddSound("WrongCombination","Play_WrongCombination")
end

function CLightPedestalsManagerComponent:AddLightPedestal(CLuaGameObject, Event, RuneLuaGameObject)
	local l_LightPedestal = CLightPedestalComponent(CLuaGameObject, Event, RuneLuaGameObject)
	g_ScriptManager:AddComponent(CLuaGameObject:GetName(),CLuaGameObject,l_LightPedestal)
	table.insert(self.m_LightPedestals,l_LightPedestal)
end

function CLightPedestalsManagerComponent:LIGHT_PEDESTAL_ONE_ACTIVATED()
	if(not self.m_CombinationDone) then 
		self:LightPedestalPressed(1)
	end 
end
function CLightPedestalsManagerComponent:LIGHT_PEDESTAL_TWO_ACTIVATED()
	if(not self.m_CombinationDone) then 
		self:LightPedestalPressed(2)
	end
end
function CLightPedestalsManagerComponent:LIGHT_PEDESTAL_THREE_ACTIVATED()
	if(not self.m_CombinationDone) then 
		self:LightPedestalPressed(3)
	end
end

function CLightPedestalsManagerComponent:LightPedestalPressed(LightPedestalIndex)
	for i=1, (#self.m_Orders) do
		local l_LightPedestalIndex = self.m_Orders[i]
		if (not self.m_Actives[l_LightPedestalIndex]) then
			if(l_LightPedestalIndex==LightPedestalIndex) then
				self.m_Actives[l_LightPedestalIndex] = true
				self.m_LightPedestals[l_LightPedestalIndex]:ActivateRune()
				self:CheckAllActives()
				return true
			else
				g_LogManager:Log("Rune pulsada incorrecta. Tocaba la "..l_LightPedestalIndex)
				self.m_LightPedestals[LightPedestalIndex]:ActivateRuneTemporarily()
				self:ResetLightPedestal()
				return false
			end
		end
	end
end

function CLightPedestalsManagerComponent:CheckAllActives()
	for i=1, (#self.m_Actives) do
		if(not self.m_Actives[i]) then return false end
	end
	g_EventManager:FireEvent("PEDESTALS_COMBINATION_DONE")
	return true
end

function CLightPedestalsManagerComponent:PEDESTALS_COMBINATION_DONE()
	self.m_CombinationDone = true
	for i=1, (#self.m_LightPedestals) do
		self.m_LightPedestals[i]:Disable()
	end
end

function CLightPedestalsManagerComponent:ResetLightPedestal()
	g_LogManager:Log("Reseteando combinacion...")
	self.m_Actives = {false,false,false}
	--self.m_LuaGameObject:PlayEvent("WrongCombination")
end

function CLightPedestalsManagerComponent:HideAllRunes()
	for i=1, (#self.m_LightPedestals) do
		self.m_LightPedestals[i]:GetRuneLuaGameObject():EnableRenderableObject(false)
	end
end

function CLightPedestalsManagerComponent:AddLight(Light)
	table.insert(self.m_Lights,Light)
end

function CLightPedestalsManagerComponent:Update(ElapsedTime)
	--if (self.m_Pedestal:IsActive())
	if (self.m_CombinationDone) and (not self.m_LightsEnabled) then
		self:AddTime(ElapsedTime)
		self:EnableLights(self:GetTimer())
		
		if((not self.m_CombinationDoneSoundPlayed) and (self:GetTimer()>self.m_CombinationDoneSoundDelay)) then
			self.m_LuaGameObject:PlayEvent("PlayerInteraction3")
			self.m_CombinationDoneSoundPlayed = true
		end
	end
end

function Lerp(a,b,k)
	return (a + (b - a)*k)
end

function CLightPedestalsManagerComponent:DisableLights()
	for i=1, (#self.m_Lights) do
		self.m_Lights[i]:SetIntensity(0.1) 
	end	
end

function CLightPedestalsManagerComponent:EnableLights(ElapsedTime)
	local l_Intensity = 0.0
	-- local l_Intensity = Lerp(0.0,1.0,ElapsedTime)
	for i=1, (#self.m_Lights) do
		local l_LightIntensity = self.m_Lights[i]:GetIntensity()
		local l_Intensity = Lerp(l_LightIntensity,1.0,ElapsedTime*0.25)
		self.m_Lights[i]:SetIntensity(l_Intensity) 
	end	
	
	if l_Intensity >= 1.0 then
		self.m_LightsEnabled = true
		--self.m_LuaGameObject:PlayEvent("LightsOn")
	end
end