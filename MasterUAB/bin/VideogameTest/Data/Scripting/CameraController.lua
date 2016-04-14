-- Checks player input for the camera
-- g_CameraControllersNames = {}
-- g_CurrentCameraIndex = 0
-- MAX_CAMERA_CONTROLLERS = 0

-- function InitializeCameraController()
	-- l_Controllers = g_CameraControllerManager:GetLUACameraControllers() 
	-- i=0
	-- for l_Controller in l_Controllers do
		-- g_CameraControllersNames[i] = l_Controller:GetName()
		-- i=i+1
		-- MAX_CAMERA_CONTROLLERS = MAX_CAMERA_CONTROLLERS + 1
	-- end
-- end

g_CameraControllerName = "FixedCamera"

function CameraController(ElapsedTime)
	l_CameraController = g_CameraControllerManager:GetCurrentCameraController()
	
	if CInputManager.GetInputManager():IsActionActive("BLOCK_CAMERA") then
		BlockCamera(l_CameraControllerManager)
	end
	
	if CInputManager.GetInputManager():IsActionActive("CENTER_CAMERA") then
		CenterCamera(l_CameraController)
	end
	
	if CInputManager.GetInputManager():IsActionActive("CHANGE_CAMERA") then
		ChangeCamera()
	end
	
	if CInputManager.GetInputManager():GetAxis("X_AXIS") then
		RotateCamera(l_CameraController,ElapsedTime)
	end
	
	if CInputManager.GetInputManager():GetAxis("Y_AXIS") then
		AddLookAtPitch(l_CameraController,ElapsedTime)
	end
	
	if CInputManager.GetInputManager():IsActionActive("ZOOM_IN_CAMERA") then
		AddZoom(l_CameraController,1.0)
	end
	
	if CInputManager.GetInputManager():IsActionActive("ZOOM_OUT_CAMERA") then
		AddZoom(l_CameraController,-1.0)
	end
end

function RotateCamera(CameraController,ElapsedTime)
	if CameraController:GetType() == CCameraController.THIRD_PERSON then
		CameraController:AddYaw(CInputManager.GetInputManager():GetAxis("X_AXIS") * ElapsedTime * g_CameraXAxisRotationSpeed)
		--CameraController:Update(Vect3f(CInputManager.GetInputManager():GetAxis("X_AXIS") * ElapsedTime * g_CameraXAxisRotationSpeed,0.0, 0.00))
	end
end

function CenterCamera(CameraController)
	CameraController:SetYaw(0.0)
	CameraController:SetPitch(0.0)
end

function ChangeCamera()
	g_CameraControllerManager:SetCurrentCameraController(g_CameraControllerName)
	if g_CameraControllerName == "FixedCamera" then
		g_CameraControllerName = "ThirdPersonCamera"
	else 	g_CameraControllerName = "FixedCamera"
	
	end
	-- g_CurrentCameraIndex = g_CurrentCameraIndex+1
	-- if g_CurrentCameraIndex > MAX_CAMERA_CONTROLLERS then
		-- g_CurrentCameraIndex = 0
	-- end
end


function BlockCamera(CameraControllerManager)
	CameraControllerManager:ChangeLockState()
end

function AddLookAtPitch(CameraController, ElapsedTime)
	if CameraController:GetType() == CCameraController.THIRD_PERSON then
		CameraController:AddLookAtPitch(CInputManager.GetInputManager():GetAxis("Y_AXIS") * ElapsedTime)
	end
end

function AddZoom(CameraController, Value)
	CameraController:AddZoom(Value)
end