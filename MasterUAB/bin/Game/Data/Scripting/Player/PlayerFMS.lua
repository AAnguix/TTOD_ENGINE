g_Walk = false
g_Run = false
P_StateTotalTime = 0.0
--Idle_State
function OnEnter_Idle_Player(PlayerComponent)
	--g_LogManager:Log("Player enters Idle")
	--local l = ((CPlayerComponent)PlayerComponent):GetSpeed()
	--g_LogManager:Log("Player "..PlayerComponent:GetType())
	-- g_LogManager:Log("Speed "..PlayerComponent:GetSpeed())
	-- g_LogManager:Log("Health "..PlayerComponent:GetHealth())
end

function OnUpdate_Idle_Player(Player, ElapsedTime)
	-- if (CInputManager.GetInputManager():IsActionActive("PLAYER_WALK_BACK")) then
		-- Player:GetAnimatorController():SetBool("Walk",false)
	-- end
end

function OnExit_Idle_Player(Player)
	--g_LogManager:Log("Player exit Idle")
end


--Attack_State
function OnEnter_Attack_Player(Player)
	g_LogManager:Log("Enters attack")
	Player:SetAttacking(true)
end

function OnUpdate_Attack_Player(Player, ElapsedTime)
	
end

function OnExit_Attack_Player(Player)
	g_LogManager:Log("Exit attack")
	Player:SetAttacking(false)
end


--Rotate_state
function OnEnter_Rotate_Player(Player)
	
end

-- function OnUpdate_Rotate_Player(Player, ElapsedTime)
	
	-- local l_PlayerForward = Vect3f(0.0,0.0,0.0)
	
	-- if Player.m_Right then
		-- l_PlayerForward = Player.m_RObject:GetRight()
	-- elseif Player.m_Left then 
		-- l_PlayerForward = Player.m_RObject:GetRight()*(-1.0)
	-- elseif Player.m_Forward then 
		-- l_PlayerForward = Player.m_RObject:GetForward()
	-- elseif Player.m_Backwards then 
		-- l_PlayerForward = Player.m_RObject:GetForward()*(-1.0)
	-- end
	
	-- local l_CameraForward = g_CameraControllerManager:GetCurrentCameraController():GetForward()
	-- local l_CurrentYaw = Player.m_RObject:GetYaw()
	-- local l_Angle = CTTODMathUtils.AngleBetweenVectors(l_CameraForward,l_PlayerForward)
	
	-- Player.m_RObject:SetYaw(CTTODMathUtils.CalculateNewAngle(l_Angle, l_CurrentYaw, Player.m_RotationVelocity, ElapsedTime))
	
	-- if math.abs(l_Angle) < 0.1 then
		-- g_Player:GetAnimatorController():SetBool("Rotate",false)	
	-- end
-- end

function OnExit_Rotate_Player(Player)
	
end


--Walk_state
function OnEnter_Walk_Player(Player)
end

function OnUpdate_Walk_Player(Player, ElapsedTime)
	
	local l_PlayerForward = Vect3f(0.0,0.0,0.0)

	if Player.m_Forward then
		l_PlayerForward = Player.m_LuaGameObject:GetForward()	
		if Player.m_Right then
			l_PlayerForward = Player.m_LuaGameObject:GetForward() + Player.m_LuaGameObject:GetRight()
			l_PlayerForward:Normalize(1)
		elseif Player.m_Left then
			l_PlayerForward = Player.m_LuaGameObject:GetForward() +(Player.m_LuaGameObject:GetRight()*(-1.0))
			l_PlayerForward:Normalize(1)
		end	
	elseif Player.m_Backwards then
		l_PlayerForward = Player.m_LuaGameObject:GetForward()*(-1.0)
		if Player.m_Right then
			l_PlayerForward = (Player.m_LuaGameObject:GetForward()*(-1.0)) + Player.m_LuaGameObject:GetRight()
			l_PlayerForward:Normalize(1)
		elseif Player.m_Left then
			l_PlayerForward = (Player.m_LuaGameObject:GetForward()*(-1.0)) +(Player.m_LuaGameObject:GetRight()*(-1.0))
			l_PlayerForward:Normalize(1)
		end	
	elseif Player.m_Right then
		l_PlayerForward = Player.m_LuaGameObject:GetRight()
	elseif Player.m_Left then 
		l_PlayerForward = Player.m_LuaGameObject:GetRight()*(-1.0) 	
	end
	
	local l_CameraForward = g_CameraControllerManager:GetCurrentCameraController():GetForward()
	local l_CurrentYaw = Player.m_LuaGameObject:GetYaw()
	local l_Angle = CTTODMathUtils.AngleBetweenVectors(l_CameraForward,l_PlayerForward)
	
	if math.abs(l_Angle) > 0.1 then
		local FinalYaw = CTTODMathUtils.CalculateNewAngle(l_Angle, l_CurrentYaw, Player.m_RotationVelocity, ElapsedTime)
		Player.m_LuaGameObject:SetYaw(FinalYaw)
	end
end

function OnExit_Walk_Player(Player)
	
end

--Block_state
function OnEnter_Block_Player(Player)
	g_LogManager:Log("PLayer enters BlockState")
	P_StateTotalTime = 0.0
	Player:SetBlockingState(true)
end

function OnUpdate_Block_Player(Player, ElapsedTime)
	P_StateTotalTime = P_StateTotalTime + ElapsedTime
end

function OnExit_Block_Player(Player)
	Player:SetBlockingState(false)
	g_LogManager:Log(P_StateTotalTime.." Tiempo total dentro del estado BlockState")
end