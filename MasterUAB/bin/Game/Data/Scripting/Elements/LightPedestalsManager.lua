class 'CLightPedestalsManagerComponent' (CLUAComponent)
function CLightPedestalsManagerComponent:__init(CLuaGameObject, StoneGateLuaGameObject)
CLUAComponent.__init(self,"LightManagerScript")	
	self.m_LuaGameObject = CLuaGameObject
	self.m_StoneGateLuaGameObject = StoneGateLuaGameObject 
	self.m_Lights = {}
	self.m_StartLightIntensities = {}
	self.m_EndLightIntensities = {}
	self.m_LightPedestals = {}
	
	self.m_CombinationDoneSoundDelay = 1.0
	self.m_LightsOnSoundDelay = 1.0
	self.m_StoneGateDisplacement = Vect3f(0.2,0.0,0.0)
	self.m_StoneGateTimeMoving = 2.0
	self.m_Orders = {3,1,2}
	
	g_SoundManager:AddComponent(self.m_LuaGameObject:GetName().."_AudioSource", self.m_LuaGameObject)
	self.m_LuaGameObject:AddSound("RightCombination","Play_RightCombination")
	self.m_LuaGameObject:AddSound("WrongCombination","Play_WrongCombination")
	self.m_LuaGameObject:AddSound("LightsOn","Play_LightsOn")
	self.m_LuaGameObject:AddSound("StoneGate","Play_StoneDoor")
		
	g_EventManager:Subscribe(self, "LIGHT_PEDESTAL_ONE_ACTIVATED")
	g_EventManager:Subscribe(self, "LIGHT_PEDESTAL_TWO_ACTIVATED")
	g_EventManager:Subscribe(self, "LIGHT_PEDESTAL_THREE_ACTIVATED")
	g_EventManager:Subscribe(self, "PEDESTALS_COMBINATION_DONE")
	
	self:Init()
end

function CLightPedestalsManagerComponent:Init()
	self.m_LightsEnabled = false
	self.m_CombinationDone = false 
	self.m_CombinationDoneSoundPlayed = false
	self.m_LightsOnSoundPlayed = false
	self.m_StoneGateSoundPlayed = false
	self.m_Actives = {false,false,false}
end

function CLightPedestalsManagerComponent:Reset()
	self:Init()
	for i=1, (#self.m_LightPedestals) do
		self.m_LightPedestals[i].m_RuneLuaGameObject:EnableRenderableObject(false)
		self.m_LightPedestals[i]:Enable()
	end
	
	for x=1, (#self.m_Lights) do
		local l_Intensity = self.m_StartLightIntensities[self.m_Lights[x]:GetName()]
		self.m_Lights[x]:SetIntensity(l_Intensity)
	end
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
	self.m_Actives = {false,false,false}
	self.m_LuaGameObject:PlayEvent("WrongCombination")
end

function CLightPedestalsManagerComponent:HideAllRunes()
	for i=1, (#self.m_LightPedestals) do
		self.m_LightPedestals[i]:GetRuneLuaGameObject():EnableRenderableObject(false)
	end
end

function CLightPedestalsManagerComponent:AddLight(Light, Intensity)
	table.insert(self.m_Lights,Light)
	self.m_StartLightIntensities[Light:GetName()] = Light:GetIntensity()
	self.m_EndLightIntensities[Light:GetName()] = Intensity
end

function CLightPedestalsManagerComponent:Update(ElapsedTime)
	--if (self.m_Pedestal:IsActive())
	if (self.m_CombinationDone) and (not self.m_LightsEnabled) then
		self:AddTime(ElapsedTime)
		if((not self.m_CombinationDoneSoundPlayed) and (self:GetTimer()>self.m_CombinationDoneSoundDelay)) then
			self:PlayCombinationDoneSound()
		end
		if(self.m_CombinationDoneSoundPlayed) then
			if ((not self.m_LightsOnSoundPlayed)and(self:GetTimer()>self.m_LightsOnSoundDelay)) then
				self:PlayLightsOnSound()
			elseif(self.m_LightsOnSoundPlayed and (not self.m_LightsEnabled)) then
				self:EnableLights(self:GetTimer()*0.05)
			end
		end
	elseif (self.m_LightsEnabled) then
		self:AddTime(ElapsedTime)
		self:MoveGate(ElapsedTime)
	end
end

function CLightPedestalsManagerComponent:PlayCombinationDoneSound()
	self.m_LuaGameObject:PlayEvent("RightCombination")
	self.m_CombinationDoneSoundPlayed = true
	self:ResetTimer()
end

function CLightPedestalsManagerComponent:PlayLightsOnSound()
	self.m_LuaGameObject:PlayEvent("LightsOn")
	self.m_LightsOnSoundPlayed = true
	self:ResetTimer()
end

function CLightPedestalsManagerComponent:MoveGate(ElapsedTime)
	if(not self.m_StoneGateSoundPlayed) then
		self.m_LuaGameObject:PlayEvent("StoneGate")
		self.m_StoneGateSoundPlayed = true
	end
	if(self:GetTimer()<self.m_StoneGateTimeMoving) then
		local l_Displacement = self.m_StoneGateDisplacement*ElapsedTime
		self.m_StoneGateLuaGameObject:SetPosition(self.m_StoneGateLuaGameObject:GetPosition()+l_Displacement)
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

function CLightPedestalsManagerComponent:EnableLights(Timer)
	local l_Intensity = 0.0
	local l_LightsModified = 0
	
	for i=1, (#self.m_Lights) do
		local l_CurrentLightIntensity = self.m_Lights[i]:GetIntensity()
		local l_EndLightIntensity = self.m_EndLightIntensities[self.m_Lights[i]:GetName()]
		
		if(math.abs(l_EndLightIntensity-l_CurrentLightIntensity)<0.0001) then
			self.m_Lights[i]:SetIntensity(l_EndLightIntensity)
		elseif (l_CurrentLightIntensity < l_EndLightIntensity) then
			local l_Intensity = Lerp(l_CurrentLightIntensity,l_EndLightIntensity,Timer)
			self.m_Lights[i]:SetIntensity(l_Intensity)
			l_LightsModified = l_LightsModified+1
		end
	end	
	
	if l_LightsModified == 0 then
		self.m_LightsEnabled = true
		self:ResetTimer()
	end
	
	l_LightsModified = 0
end