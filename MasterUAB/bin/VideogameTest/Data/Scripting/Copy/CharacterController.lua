function PlayerController(ElapsedTime)
	--RaiseAnimation("solid","Personaje",0)

	if CInputManager.get_input_manager():is_action_active("MOVE_FWD") then
		MoveForward(ElapsedTime)
	end
	
	if CInputManager.get_input_manager():is_action_active("MOVE_BACK") then
		MoveBack()
	end
	
	if CInputManager.get_input_manager():is_action_active("STRAFE_RIGHT") then
		MoveRight()
	end
	
	if CInputManager.get_input_manager():is_action_active("STRAFE_LEFT") then
		MoveLeft()
	end
	
	if CInputManager.get_input_manager():is_action_active("BLOCK_CAMERA") then
		BlockCamera()
	end
end

function MoveForward(ElapsedTime)
	-- local l_Engine = CEngine.get_singleton()
	-- local l_CameraControllerManager = l_Engine:get_camera_controller_manager()
	-- local l_CameraController = l_CameraControllerManager:get_current_camera_controller()
	-- l_CameraController:move(1,1,false,1)
end

function MoveBack()
	
end

function MoveRight()
	
end

function MoveLeft()
	
end

function BlockCamera()
	local l_Engine = CEngine.get_singleton()
	local l_CameraControllerManager = l_Engine:get_camera_controller_manager()
	l_CameraControllerManager:change_lock_state()
end

function RaiseAnimation(RenderableObjectManager,CharacterName,AnimationID)
-- local l_Engine = CEngine.get_singleton()
-- local l_Layers = l_Engine:get_layer_manager()
-- local l_ROManager = l_Layers:get_resource(RenderableObjectManager)
-- local l_AnimatedModel = l_ROManager:get_resource(CharacterName)
-- l_AnimatedModel:execute_action(AnimationID,0.1,0.2,1.0,true)
end