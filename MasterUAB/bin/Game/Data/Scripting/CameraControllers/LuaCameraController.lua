class 'CLuaCameraController'
function CLuaCameraController:__init()
	self:SubscribeEvents()
	self.m_Speed = false
end

g_CameraControllerName = "FixedCamera"

function CLuaCameraController:IsDebug()
	return self:GetCCC():GetType() == CCameraController.DEBUG
end

function CLuaCameraController:SubscribeEvents()
	g_EventManager:Subscribe(self, "ZOOM_IN_CAMERA")
	g_EventManager:Subscribe(self, "ZOOM_OUT_CAMERA")
	g_EventManager:Subscribe(self, "BLOCK_CAMERA")
	g_EventManager:Subscribe(self, "CENTER_CAMERA")
	
	g_EventManager:Subscribe(self, "CAMERA_MOVES_FORWARD")
	g_EventManager:Subscribe(self, "CAMERA_MOVES_BACKWARD")
end

function CLuaCameraController:GetCCC()
	return g_CameraControllerManager:GetCurrentCameraController()
end

function CLuaCameraController:BLOCK_CAMERA()
	g_CameraControllerManager:ChangeLockState()
end

function CLuaCameraController:CENTER_CAMERA()
	-- self:GetCCC():SetYaw(0.0)
	-- self:GetCCC():SetPitch(0.0)
	g_Engine:GetTextureManager():SaveBlackAndWhiteTexturesToFile()
end

function CLuaCameraController:ZOOM_IN_CAMERA()
	self:GetCCC():AddZoom(1.0)
end

function CLuaCameraController:ZOOM_OUT_CAMERA()
	self:GetCCC():AddZoom(-1.0)
end

function CLuaCameraController:CAMERA_MOVES_FORWARD()
	if(self:IsDebug())then
		self:GetCCC():Move(0.0, 1.0,self.m_Speed,ElapsedTime)	
	end
end
function CLuaCameraController:CAMERA_MOVES_BACKWARD()
	if(self:IsDebug())then
		self:GetCCC():Move(0.0,-1.0,self.m_Speed,ElapsedTime)
	end
end	
function CLuaCameraController:CAMERA_MOVES_UP()
	if(self:IsDebug())then
		self:GetCCC():MoveUpDown(1.0,self.m_Speed,ElapsedTime)
	end
end
function CLuaCameraController:CAMERA_MOVES_DOWN()
	if(self:IsDebug())then
		self:GetCCC():MoveUpDown(-1.0,self.m_Speed,ElapsedTime)
	end
end

function CLuaCameraController:Update()
	if self:GetCCC():GetType() == CCameraController.THIRD_PERSON then
		--ThirdPersonCamera(ElapsedTime)
	elseif self:GetCCC():GetType() == CCameraController.DEBUG then
		--DebugCamera(ElapsedTime) 
	end
end

function CLuaCameraController:ThirdPersonCamera(ElapsedTime)
	local l_CameraController = g_CameraControllerManager:GetCurrentCameraController()
end

-- function CLuaCameraController:CameraShake(ElapsedTime)
	-- local l_Rotation = Vect3f(0.0,0.0,0.0)
	-- local l_Yaw = self:GetCCC():GetYaw()
	-- local l_Pitch = self:GetCCC():GetPitch()
	-- local l_Roll = 0.0
	
	-- local l_Amplituded = 1.0
	-- local l_Frequence = 1.0
	
	-- local l_V1 = CPerlinNoise.noise3(l_Yaw/l_Frequence,l_Pitch/l_Frequence,l_Roll/l_Frequence)*l_Amplituded
	-- l_Rotation.x = l_Rotation.x + l_V1
	-- local l_V2 = CPerlinNoise.noise3(l_Yaw/l_Frequence,l_Pitch/l_Frequence+100,l_Roll/l_Frequence)*l_Amplituded
	-- l_Rotation.y = l_Rotation.y + l_V2
	-- --l_Rotation.z += CPerlinNoise.noise(l_Yaw/l_Frequence,l_Pitch/l_Frequence,l_Roll/l_Frequence+100)*l_Amplituded
	-- g_LogManager:Log(l_Rotation.x.." "..l_Rotation.y.." "..l_Rotation.z)
	
	-- self:GetCCC():AddYaw(l_Rotation.x)
	-- self:GetCCC():AddPitch(l_Rotation.y)
-- end

	-- local yaw = self:GetCCC():GetYaw()

	
	-- local pìtch = self:GetCCC():GetPitch()
	
	-- local l_Rotation = Vect3f(0.0,0.0,0.0)
	
	-- local roll = l_Camera:GetRoll()
	
	-- l_Rotation.x += CPerlinNoise.noise(yaw/l_Frequence,pìtch/l_Frequence,roll/l_Frequence)*l_Amplituded
	-- l_Rotation.y += CPerlinNoise.noise(yaw/l_Frequence,pìtch/l_Frequence+100,roll/l_Frequence)*l_Amplituded
	-- l_Rotation.z += CPerlinNoise.noise(yaw/l_Frequence,pìtch/l_Frequence,roll/l_Frequence+100)*l_Amplituded
	
	-- self:GetCCC():SetYaw(l_Rotation.y)
	-- self:GetCCC():SetPitch(l_Rotation.x)

