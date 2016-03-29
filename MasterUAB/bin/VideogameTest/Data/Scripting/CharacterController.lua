function SetPlayerCharacterDefaultState()
	GetPlayer():BlendCycle(0,1.0,0.1)
end

function CharacterController(ElapsedTime)
	l_Engine = CEngine.GetSingleton()
	l_CameraControllerManager = l_Engine:GetCameraControllerManager()
	l_CameraController = l_CameraControllerManager:GetCurrentCameraController()
	l_PhysicsManager = l_Engine:GetPhysXManager()
	
	CameraController(ElapsedTime)
	
	if ((CInputManager.GetInputManager():IsActionActive("PLAYER_WALK_FORWARD")) or 
		(CInputManager.GetInputManager():IsActionActive("PLAYER_WALK_BACK")) or 
		(CInputManager.GetInputManager():IsActionActive("PLAYER_WALK_LEFT")) or 
		(CInputManager.GetInputManager():IsActionActive("PLAYER_WALK_RIGHT")))  then
		local l_Player = GetPlayer()
		local l_Position = l_Player:GetPosition()
		l_Player:BlendCycle(2,1.0,0.1)
	end
	
	-- if ((CInputManager.GetInputManager():IsActionActive("PLAYER_WALK_FORWARD_END")) or
		-- (CInputManager.GetInputManager():IsActionActive("PLAYER_WALK_BACK_END")) or 
		-- (CInputManager.GetInputManager():IsActionActive("PLAYER_WALK_LEFT_END")) or 
		-- (CInputManager.GetInputManager():IsActionActive("PLAYER_WALK_RIGHT_END")))  then
		-- GetPlayer():ClearCycle(2,0.15)
	-- end
	
	if (CInputManager.GetInputManager():IsActionActive("PLAYER_WALK_FORWARD_END")) then
		GetPlayer():ClearCycle(2,0.1)
	end
	if (CInputManager.GetInputManager():IsActionActive("PLAYER_WALK_BACK_END")) then
		GetPlayer():ClearCycle(2,0.1)
	end
	if (CInputManager.GetInputManager():IsActionActive("PLAYER_WALK_LEFT_END")) then
		GetPlayer():ClearCycle(2,0.1)
	end
	if (CInputManager.GetInputManager():IsActionActive("PLAYER_WALK_RIGHT_END"))then
		GetPlayer():ClearCycle(2,0.1)
	end
	
	if CInputManager.GetInputManager():IsActionActive("MOVE_FWD") then
		--l_Displacement = GetDisplacement(Vect3f(0.0,0.0,0.1),false,ElapsedTime)
		--SetPlayerPosition(GetPlayer():GetPosition()+l_Displacement)
		PlayerYaw(l_CameraController)
		--SetPlayerPosition(MovePlayerForward(l_CameraController,false,ElapsedTime))
		local v = l_CameraController:GetForward()
		MovePlayerPhisics(Vect3f(v.x,0.0,v.z),false,ElapsedTime,false)
	end
	if CInputManager.GetInputManager():IsActionActive("MOVE_BACK") then
		--SetPlayerPosition(MovePlayerBackward(l_CameraController,false,ElapsedTime))
		--l_Displacement = GetDisplacement(Vect3f(0.0,0.0,-0.1),false,ElapsedTime)
		--SetPlayerPosition(GetPlayer():GetPosition()+l_Displacement)
		--l_PhysicsManager:MoveCharacterController(GetPlayer():GetName(),l_Displacement,ElapsedTime)
		PlayerYaw(l_CameraController)
		local v = l_CameraController:GetForward()
		MovePlayerPhisics(Vect3f(-v.x,0.0,-v.z),false,ElapsedTime, true)
	end
	if CInputManager.GetInputManager():IsActionActive("STRAFE_RIGHT") then
		--l_Displacement = GetDisplacement(Vect3f(-0.1,0.0,0.0),false,ElapsedTime)
		--SetPlayerPosition(GetPlayer():GetPosition()+l_Displacement)
		--l_PhysicsManager:MoveCharacterController(GetPlayer():GetName(),l_Displacement,ElapsedTime)
		PlayerYaw(l_CameraController)	
		local v = l_CameraController:GetRight()
		MovePlayerPhisics(Vect3f(-v.x,-v.y,-v.z),false,ElapsedTime, true)
	end
	if CInputManager.GetInputManager():IsActionActive("STRAFE_LEFT") then
		--l_Displacement = GetDisplacement(Vect3f(-0.1,0.0,0.0),false,ElapsedTime)
		--SetPlayerPosition(GetPlayer():GetPosition()+l_Displacement)
		--l_PhysicsManager:MoveCharacterController(GetPlayer():GetName(),l_Displacement,ElapsedTime)
		PlayerYaw(l_CameraController)	
		local v = l_CameraController:GetRight()
		MovePlayerPhisics(Vect3f(v.x,v.y,v.z),false,ElapsedTime, true)
	end
	
	if CInputManager.GetInputManager():IsActionActive("PLAYER_JUMP") then
		Jump(true,ElapsedTime)
	end
	
	--Gravity
	MovePlayerPhisics(Vect3f(0.0,-10.0,0.0),false,ElapsedTime)
	
	--CEngine.GetSingleton():GetPhysXManager():MoveCharacterController(GetPlayer():GetName(),Vect3f(0.0,-10.0,0.0),ElapsedTime)
	
	-- if CInputManager.GetInputManager():IsActionActive("ANIMATION_2") then
		-- GetPlayer():ExecuteAction(1,0.1,0.2,1.0,false)		--GetPlayer():BlendCycle(1,0.1,1.0)
	-- end
	
	-- if CInputManager.GetInputManager():IsActionActive("ANIMATION_3") then
		-- GetPlayer():ExecuteAction(2,0.1,0.2,1.0,false)
	-- end
	
	-- if CInputManager.GetInputManager():IsActionActive("ANIMATION_4") then
		-- GetPlayer():ExecuteAction(3,0.1,0.2,1.0,false)
	-- end
	
	-- if CInputManager.GetInputManager():IsActionActive("MOVE_FWD") then
		-- MoveForward(l_CameraController,ElapsedTime)
	-- end
	
	-- if CInputManager.GetInputManager():IsActionActive("MOVE_BACK") then
		-- MoveBack(l_CameraController,ElapsedTime)
	-- end
	
	-- if CInputManager.GetInputManager():IsActionActive("STRAFE_RIGHT") then
		-- MoveRight(l_CameraController,ElapsedTime)
	-- end
	
	-- if CInputManager.GetInputManager():IsActionActive("STRAFE_LEFT") then
		-- MoveLeft(l_CameraController,ElapsedTime)
	-- end
	
	-- if CInputManager.GetInputManager():IsActionActive("MOVE_UP") then
		-- MoveUp(l_CameraController,ElapsedTime)
	-- end
	
	-- if CInputManager.GetInputManager():IsActionActive("MOVE_DOWN") then
		-- MoveDown(l_CameraController,ElapsedTime)
	-- end
	
	--Enemy=CEngine.GetSingleton():GetLayerManager():GetResource("solid"):GetResource("catapult")
	-- FollowPlayer(ElapsedTime, Enemy, g_EnemysSpeed)
	l_Points = {}
	l_Points[0] = Vect3f(0.0,0.0,0.0)
	l_Points[1] = Vect3f(3.0,0.0,0.0)
	l_Points[2] = Vect3f(3.0,0.0,0.5)
	--PatrolWayPoints(l_Points, ElapsedTime, Enemy, g_EnemysSpeed, 0.2)
end

function MoveForward(CameraController,ElapsedTime)
	CameraController:Move(0.0,CInputManager.GetInputManager():GetAxis("MOVE_FWD")*ElapsedTime,false,ElapsedTime)
end

function MoveBack(CameraController,ElapsedTime)
	CameraController:Move(0.0,CInputManager.GetInputManager():GetAxis("MOVE_BACK")*ElapsedTime,false,ElapsedTime)
end

function MoveRight(CameraController,ElapsedTime)
	CameraController:Move(CInputManager.GetInputManager():GetAxis("STRAFE")*ElapsedTime,0.0,false,ElapsedTime)
end

function MoveLeft(CameraController,ElapsedTime)
	CameraController:Move(CInputManager.GetInputManager():GetAxis("STRAFE")*ElapsedTime,0.0,false,ElapsedTime)
end

function MoveUp(CameraController,ElapsedTime)
	CameraController:MoveUpDown(CInputManager.GetInputManager():GetAxis("MOVE_UP")*ElapsedTime,false,ElapsedTime)
end

function MoveDown(CameraController,ElapsedTime)
	CameraController:MoveUpDown(CInputManager.GetInputManager():GetAxis("MOVE_DOWN")*ElapsedTime,false,ElapsedTime)
end

function GetCameraPosition() 
	local l_CController = CEngine.GetSingleton():GetCameraControllerManager():get_current_camera_controller()
	return l_CController:get_position()
end

function MovePlayer(Displacement, Speed, ElapsedTime)
	
	local l_PlayerPos = GetPlayer():GetPosition()
	local l_ConstantSpeed=ElapsedTime*g_PlayerSpeed
	
	if Speed then
		l_ConstantSpeed=l_ConstantSpeed*g_PlayerFastSpeed
	end
	
	if Displacement:SquaredLength() > 0 then
		Displacement:Normalize(1)
		Displacement = Displacement*l_ConstantSpeed
		l_PlayerPos = l_PlayerPos + Displacement
	end
	
	return l_PlayerPos
end

function GetDisplacement(Displacement, Speed, ElapsedTime)
	
	local l_ConstantSpeed=ElapsedTime*g_PlayerSpeed
	
	if Speed then
		l_ConstantSpeed=l_ConstantSpeed*g_PlayerFastSpeed
	end
	
	if Displacement:SquaredLength() > 0 then
		Displacement:Normalize(1)
		Displacement = Displacement*l_ConstantSpeed
	end
	
	return Displacement
end

function SetPlayerPosition(Position)
	GetPlayer():SetPosition(Position)
end

--Jesus
function PlayerYaw (CameraController)
	GetPlayer():SetYaw(CameraController:GetYaw())
end

function MovePlayerForward(CameraController,Speed, ElapsedTime)
	
	local l_PlayerPos = GetPlayer():GetPosition()
	local l_ConstantSpeed=ElapsedTime*g_PlayerSpeed
	
	if Speed then
		l_ConstantSpeed=l_ConstantSpeed*g_PlayerFastSpeed
	end
	--l_CameraForward(sin(l_CameraYaw), 0.0f, cos(l_CameraYaw));
	local Displacement = CameraController:GetForward()
	
	if Displacement:SquaredLength() > 0 then
		 Displacement:Normalize(1)
		 Displacement = Displacement*l_ConstantSpeed
		 l_Engine:GetPhysXManager():MoveCharacterController(GetPlayer():GetName(),Vect3f(Displacement.x,Displacement.y,Displacement.z),ElapsedTime)
		 l_PlayerPos = l_PlayerPos + Displacement
	 end
	
	return l_PlayerPos
end

function MovePlayerPhisics(Displacement,Speed, ElapsedTime, Slow)
	local MoveSpeed = g_PlayerSpeed
	if Slow then 
		MoveSpeed = g_PlayerStrafeSpeed
	end
	--l_Engine = CEngine.GetSingleton()
	local l_ConstantSpeed=ElapsedTime*MoveSpeed
	
	if Speed then
		l_ConstantSpeed=l_ConstantSpeed*g_PlayerFastSpeed
	end

	--local Displacement = CameraController:GetForward()
	
	if Displacement:SquaredLength() > 0 then
		 Displacement:Normalize(1)
		 Displacement = Displacement*l_ConstantSpeed
		l_Position = g_PhysXManager:MoveCharacterController(GetPlayer():GetName(),Displacement,ElapsedTime)
		GetPlayer():SetPosition(l_Position)
	end
end

function Jump(Speed, ElapsedTime)

	l_Engine = CEngine.GetSingleton()
	local l_ConstantSpeed=ElapsedTime*g_PlayerSpeed
	
	if Speed then
		l_ConstantSpeed=l_ConstantSpeed*g_PlayerFastSpeed
	end

	local Displacement = Vect3f(0.0,12.0,5.0)
	
	if Displacement:SquaredLength() > 0 then
		 Displacement:Normalize(1)
		 Displacement = Displacement*l_ConstantSpeed
		l_Position = l_Engine:GetPhysXManager():MoveCharacterController(GetPlayer():GetName(),Displacement,ElapsedTime)
		GetPlayer():SetPosition(l_Position)
	end
end

function MovePlayerBackward(CameraController,Speed, ElapsedTime)
	
	local l_PlayerPos = GetPlayer():GetPosition()
	local l_ConstantSpeed=ElapsedTime*g_PlayerStrafeSpeed
	
	if Speed then
		l_ConstantSpeed=l_ConstantSpeed*g_PlayerStrafeFastSpeed
	end
	--l_CameraForward(sin(l_CameraYaw), 0.0f, cos(l_CameraYaw));
	local Displacement = CameraController:GetForward()
	if Displacement:SquaredLength() > 0 then
		Displacement:Normalize(1)
		Displacement = Displacement*l_ConstantSpeed
		l_PlayerPos = l_PlayerPos - Displacement
	end
	
	return l_PlayerPos
end

function MovePlayerRight(CameraController,Speed, ElapsedTime)
	
	local l_PlayerPos = GetPlayer():GetPosition()
	local l_ConstantSpeed=ElapsedTime*g_PlayerStrafeSpeed
	
	if Speed then
		l_ConstantSpeed=l_ConstantSpeed*g_PlayerStrafeFastSpeed
	end
	--l_CameraForward(sin(l_CameraYaw), 0.0f, cos(l_CameraYaw));
	local Displacement = CameraController:GetRight()
	if Displacement:SquaredLength() > 0 then
		Displacement:Normalize()
		Displacement = Displacement*l_ConstantSpeed
		l_PlayerPos = l_PlayerPos - Displacement
	end
	return l_PlayerPos
end

function MovePlayerLeft(CameraController,Speed, ElapsedTime)
	
	local l_PlayerPos = GetPlayer():GetPosition()
	local l_ConstantSpeed=ElapsedTime*g_PlayerStrafeSpeed
	
	if Speed then
		l_ConstantSpeed=l_ConstantSpeed*g_PlayerStrafeFastSpeed
	end
	--l_CameraForward(sin(l_CameraYaw), 0.0f, cos(l_CameraYaw));
	local Displacement = CameraController:GetRight()
	if Displacement:SquaredLength() > 0 then
		Displacement:Normalize()
		Displacement = Displacement*l_ConstantSpeed
		l_PlayerPos = l_PlayerPos + Displacement
	end
	
	return l_PlayerPos
end

