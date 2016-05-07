function SetPlayerCharacterDefaultState()
	GetPlayer():BlendCycle(0,1.0,0.1)
end

CharBlock = false

l_CCVelocity = Vect3f(0.0,0.0,0.0)

function CharacterController(ElapsedTime)

	g_Walk = false
	g_Run = false
	
	l_CameraController = g_CameraControllerManager:GetCurrentCameraController()
	
	if CInputManager.GetInputManager():IsActionActive("PRUEBA") then
		
		if (CharBlock==true) then		
			CharBlock = false
		elseif (CharBlock==false) then		
			CharBlock = true
		end
	end
	
	if (CharBlock==false) then
	
		if CInputManager.GetInputManager():IsActionActive("PLAYER_ATTACKS") then	
			GetPlayer():GetAnimatorController():SetTrigger("Attack");	
		end
		if CInputManager.GetInputManager():IsActionActive("PLAYER_BLOCKS") then	
			GetPlayer():GetAnimatorController():SetTrigger("Block");	
		end
	
		if ((CInputManager.GetInputManager():IsActionActive("PLAYER_WALK_FORWARD")) or 
		(CInputManager.GetInputManager():IsActionActive("PLAYER_WALK_BACK")) or 
		(CInputManager.GetInputManager():IsActionActive("PLAYER_WALK_LEFT")) or 
		(CInputManager.GetInputManager():IsActionActive("PLAYER_WALK_RIGHT")))  then
			local l_Player = GetPlayer()
			local l_Position = l_Player:GetPosition()
			g_Walk = true
			--l_Player:BlendCycle(2,1.0,0.1)
			--g_LogManager:Log("Entrado Aca")
		--else
			--GetPlayer():ClearCycle(2,0.1)
			--GetPlayer():BlendCycle(0,1.0,0.1)
		end
		
		l_CCVelocity.x = 0;
		l_CCVelocity.z = 0;
		
		l_Speed = 3
		
		if CInputManager.GetInputManager():IsActionActive("MOVE_FWD") then			
			MatchPlayerYawToCameraYaw(l_CameraController)				
			local v = l_CameraController:GetForward()
			l_CCVelocity = l_CCVelocity + v * l_Speed
		end
		
		if CInputManager.GetInputManager():IsActionActive("MOVE_BACK") then				
			MatchPlayerYawToCameraYaw(l_CameraController)
			local v = l_CameraController:GetForward()
			l_CCVelocity = l_CCVelocity - v * l_Speed
		end
		
		if CInputManager.GetInputManager():IsActionActive("STRAFE_RIGHT") then
			
			MatchPlayerYawToCameraYaw(l_CameraController)	
			local v = l_CameraController:GetRight()
			l_CCVelocity = l_CCVelocity - v * l_Speed
		end
		
		if CInputManager.GetInputManager():IsActionActive("STRAFE_LEFT") then
			MatchPlayerYawToCameraYaw(l_CameraController)	
			local v = l_CameraController:GetRight()
			l_CCVelocity = l_CCVelocity + v * l_Speed
		end
		
		if CInputManager.GetInputManager():IsActionActive("PRUEBA") then
			--Jump(true,ElapsedTime)
			l_CCVelocity.y = 3
		end
		
		l_CCVelocity = l_CCVelocity + Vect3f(0,-10.0,0) * ElapsedTime
		
		--g_PhysXManager:DisplacementCharacterController(GetPlayer():GetName(), GetMoveBuffer(), ElapsedTime,3)
		
		if ElapsedTime>0.0 then
			l_CCVelocity = g_PhysXManager:DisplacementCharacterController(GetPlayer():GetName(), (l_CCVelocity * ElapsedTime), ElapsedTime)
		else 
			l_CCVelocity = g_PhysXManager:DisplacementCharacterController(GetPlayer():GetName(), (l_CCVelocity), ElapsedTime)
		end 
		--CalculateVelocity(GetMoveBuffer(),ElapsedTime)
		--l_Displacement = CalculateDisplacement(GetMoveBuffer(),g_Gravity,ElapsedTime)
		--MovePlayerPhisics(l_Displacement,false,ElapsedTime,false)
		
	end --END CharBlock
	
	GetPlayer():GetAnimatorController():SetBool("Walk", g_Walk);
   	GetPlayer():GetAnimatorController():SetBool("Run", g_Run);
	
end

function CalculateVelocity(Distance,Time)
	return Distance/Time
end

function CalculateDisplacement(InitialVelocity,Acceleration,Time)
	return (InitialVelocity*Time) + ((Acceleration*0.5)*(Time*Time))
end


function GetCameraPosition() 
	local l_CController = CEngine.GetSingleton():GetCameraControllerManager():get_current_camera_controller()
	return l_CController:get_position()
end

function SetPlayerPosition(Position)
	GetPlayer():SetPosition(Position)
end

function MatchPlayerYawToCameraYaw (CameraController)
	GetPlayer():SetYaw(CameraController:GetYaw())
end

function MovePlayerPhisics(Displacement,Speed, ElapsedTime, Slow)
	local MoveSpeed = g_PlayerSpeed
	if Slow then 
		MoveSpeed = g_PlayerStrafeSpeed
	end
	
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

	local l_ConstantSpeed=ElapsedTime*g_PlayerSpeed
	
	if Speed then
		l_ConstantSpeed=l_ConstantSpeed*g_PlayerFastSpeed
	end

	local Displacement = Vect3f(0.0,12.0,5.0)
	
	if Displacement:SquaredLength() > 0 then
		 Displacement:Normalize(1)
		 Displacement = Displacement*l_ConstantSpeed
		l_Position = g_PhysXManager:MoveCharacterController(GetPlayer():GetName(),Displacement,ElapsedTime)
		GetPlayer():SetPosition(l_Position)
	end
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

