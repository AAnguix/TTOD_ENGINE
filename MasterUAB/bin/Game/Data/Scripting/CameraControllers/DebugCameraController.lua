function UpdateDebugCameraController(ElapsedTime)
	
	CheckCameraControllerInput()
	
	local l_CameraController = g_CameraControllerManager:GetCurrentCameraController()
	
	if CInputManager.GetInputManager():GetAxis("X_AXIS") then
		RotateDebugCamera(l_CameraController,ElapsedTime)
	end
	
	if CInputManager.GetInputManager():GetAxis("Y_AXIS") then
		AddPitchDebugCamera(l_CameraController,ElapsedTime)
	end
	
	local l_Speed = false
	
	if CInputManager.GetInputManager():IsActionActive("MOVE_FWD") then
		l_CameraController:Move(0.0, 1.0,l_Speed,ElapsedTime)
	end
	
	if CInputManager.GetInputManager():IsActionActive("MOVE_BACK") then
		l_CameraController:Move(0.0,-1.0,l_Speed,ElapsedTime)
	end
	
	if CInputManager.GetInputManager():IsActionActive("MOVE_UP") then
		l_CameraController:MoveUpDown(1.0,l_Speed,ElapsedTime)
	elseif CInputManager.GetInputManager():IsActionActive("MOVE_DOWN") then
		l_CameraController:MoveUpDown(-1.0,l_Speed,ElapsedTime)
	end
end

function RotateDebugCamera(CameraController,ElapsedTime)
	CameraController:AddYaw(CInputManager.GetInputManager():GetAxis("X_AXIS") * ElapsedTime * 0.3)
end

function AddPitchDebugCamera(CameraController, ElapsedTime)
	CameraController:AddPitch(CInputManager.GetInputManager():GetAxis("Y_AXIS") * ElapsedTime * 0.3)
end

function MoveForwardDebugCamera()

end
