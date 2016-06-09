class 'CLightManager' (CLUAComponent)
function CLightManager:__init(Pedestal)
CLUAComponent.__init(self,"LightManagerScript")	
	self.m_LightsEnabled = false
	self.m_Lights = {}
	self.m_Pedestal = Pedestal
	self.m_Time = 0.0
end

function CLightManager:AddLight(Light)
	table.insert(self.m_Lights,Light)
end

function CLightManager:Update(ElapsedTime)
	--if (self.m_Pedestal:IsActive())
	if (self.m_Pedestal:IsActive()) and (not self.m_LightsEnabled) then
		self.m_Time = self.m_Time + ElapsedTime
		self:EnableLights(self.m_Time)
	end
end

function Lerp(a,b,k)
	return (a + (b - a)*k)
end

function CLightManager:EnableLights(ElapsedTime)
	local l_Intensity = Lerp(0.0,1.0,ElapsedTime)
	for i=1, (#self.m_Lights) do
		self.m_Lights[i]:SetIntensity(l_Intensity) 
	end	
	
	if l_Intensity >= 1.0 then
		self.m_LightsEnabled = true
	end
end