-- Checks player input for the camera

function CameraController(ElapsedTime)
	l_CameraController = CEngine.GetSingleton():GetCameraControllerManager():GetCurrentCameraController()
	if CInputManager.GetInputManager():IsActionActive("BLOCK_CAMERA") then
			BlockCamera(l_CameraControllerManager)
	end
	if CInputManager.GetInputManager():IsActionActive("CENTER_CAMERA") then
		CenterCamera(l_CameraController)
	end
	if CInputManager.GetInputManager():GetAxis("X_AXIS") then
		RotateCamera(l_CameraController,ElapsedTime)
	end
end

function RotateCamera(CameraController,ElapsedTime)
	if CameraController:GetType() == CCameraController.FIXED then
		CameraController:Update(Vect3f(CInputManager.GetInputManager():GetAxis("X_AXIS") * ElapsedTime * g_CameraXAxisRotationSpeed,0.0, 0.00))
	end
end

function CenterCamera(CameraController)
	CameraController:SetYaw(0.0)
	CameraController:SetPitch(0.0)
end

function BlockCamera(CameraControllerManager)
	CameraControllerManager:ChangeLockState()
end