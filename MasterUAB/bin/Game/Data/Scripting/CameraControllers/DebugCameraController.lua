function UpdateDebugCameraController(ElapsedTime)
	
	CheckCameraControllerInput()
	
	if CInputManager.GetInputManager():GetAxis("X_AXIS") then
		RotateDebugCamera(g_CameraController,ElapsedTime)
	end
	
	if CInputManager.GetInputManager():GetAxis("Y_AXIS") then
		AddPitchDebugCamera(g_CameraController,ElapsedTime)
	end
	
	local l_Speed = false
	
	if CInputManager.GetInputManager():IsActionActive("MOVE_FWD") then
		g_CameraController:Move(0.0, 1.0,l_Speed,ElapsedTime)
	end
	
	if CInputManager.GetInputManager():IsActionActive("MOVE_BACK") then
		g_CameraController:Move(0.0,-1.0,l_Speed,ElapsedTime)
	end
	
	if CInputManager.GetInputManager():IsActionActive("MOVE_UP") then
		g_CameraController:MoveUpDown(1.0,l_Speed,ElapsedTime)
	elseif CInputManager.GetInputManager():IsActionActive("MOVE_DOWN") then
		g_CameraController:MoveUpDown(-1.0,l_Speed,ElapsedTime)
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
