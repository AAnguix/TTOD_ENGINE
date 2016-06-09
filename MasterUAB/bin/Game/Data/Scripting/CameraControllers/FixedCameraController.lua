function UpdateFixedCameraController()
	CheckCameraControllerInput()
	
	if CInputManager.GetInputManager():GetAxis("X_AXIS") then
		RotateDebugCamera(g_CameraController,ElapsedTime)
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

function RotateDebugCamera(CameraController,ElapsedTime)
	CameraController:AddYaw(CInputManager.GetInputManager():GetAxis("X_AXIS") * ElapsedTime * g_CameraXAxisRotationSpeed)
end