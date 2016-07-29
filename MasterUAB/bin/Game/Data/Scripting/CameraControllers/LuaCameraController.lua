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
	self:GetCCC():SetYaw(0.0)
	self:GetCCC():SetPitch(0.0)
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


