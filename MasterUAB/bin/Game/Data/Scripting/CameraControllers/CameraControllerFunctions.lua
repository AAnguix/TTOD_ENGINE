function UpdateCameraController(ElapsedTime)
	if g_CameraController:GetType() == CCameraController.THIRD_PERSON then
		UpdateThirdPersonCameraController(ElapsedTime)
	elseif g_CameraController:GetType() == CCameraController.DEBUG then
		UpdateDebugCameraController(ElapsedTime) 
	end
end

function CheckCameraControllerInput()
	if CInputManager.GetInputManager():IsActionActive("BLOCK_CAMERA") then
		BlockCamera(g_CameraControllerManager)
	end
	
	if CInputManager.GetInputManager():IsActionActive("CENTER_CAMERA") then
		CenterCamera(g_CameraController)
	end
	
	if CInputManager.GetInputManager():IsActionActive("CHANGE_CAMERA") then
		ChangeCamera()
	end
end

function CenterCamera(CameraController)
	CameraController:SetYaw(0.0)
	CameraController:SetPitch(0.0)
end

function BlockCamera(CameraControllerManager)
	CameraControllerManager:ChangeLockState()
end

function ChangeCamera()
	g_CameraControllerManager:NextCameraController()
end