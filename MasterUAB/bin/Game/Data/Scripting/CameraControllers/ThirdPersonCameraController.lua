g_CameraXAxisRotationSpeed=-0.3

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

function UpdateThirdPersonCameraController(ElapsedTime)
	
	CheckCameraControllerInput()
	
	if CInputManager.GetInputManager():GetAxis("X_AXIS") then
		RotateThirdPersonCamera(g_CameraController,ElapsedTime)
	end
	
	if CInputManager.GetInputManager():GetAxis("Y_AXIS") then
		AddLookAtPitch(g_CameraController,ElapsedTime)
	end
	
	if CInputManager.GetInputManager():IsActionActive("ZOOM_IN_CAMERA") then
		AddZoom(g_CameraController,1.0)
	end
	
	if CInputManager.GetInputManager():IsActionActive("ZOOM_OUT_CAMERA") then
		AddZoom(g_CameraController,-1.0)
	end
end

function RotateThirdPersonCamera(CameraController,ElapsedTime)
	CameraController:AddYaw(CInputManager.GetInputManager():GetAxis("X_AXIS") * ElapsedTime * g_CameraXAxisRotationSpeed)
	--CameraController:Update(Vect3f(CInputManager.GetInputManager():GetAxis("X_AXIS") * ElapsedTime * g_CameraXAxisRotationSpeed,0.0, 0.00))
end

function AddLookAtPitch(CameraController, ElapsedTime)
	CameraController:AddLookAtPitch(CInputManager.GetInputManager():GetAxis("Y_AXIS") * ElapsedTime)
end

function AddZoom(CameraController, Value)
	CameraController:AddZoom(Value)
end