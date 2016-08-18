g_Walk = false
g_Run = false
P_StateTotalTime = 0.0
--Idle_State
function OnEnter_Idle_Player(PlayerComponent)
	--g_LogManager:Log("Player enters Idle")
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
	--g_LogManager:Log("Enters attack")
	Player:SetAttacking(true)
	Player:SetAttackFinished(false)
	
	local l_CameraForward = g_CameraControllerManager:GetCurrentCameraController():GetForward()
	Player:SetAttackDirection(l_CameraForward)
	
	-- g_Engine:GetRenderManager():GetContextManager():SetFullScreen(true, 1920, 1080)
	
	-- Player:TakeDamage(nil,400)
end

function OnUpdate_Attack_Player(Player, ElapsedTime)
	Player:CalculateAttackDirection(ElapsedTime)
	local l_PlayerForward = Player.m_LuaGameObject:GetForward()    
	Player.m_Velocity = Player.m_Velocity + (l_PlayerForward*Player.m_AttackDisplacement)
end

function OnExit_Attack_Player(Player)
	--g_LogManager:Log("Exit attack")
	Player:SetAttacking(false)
	Player:SetAttackFinished(true)
end

--Walk_state
function OnEnter_Walk_Player(Player)
	-- g_Engine:GetRenderManager():GetContextManager():SetFullScreen(false, 1920, 1080)
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

--Tossed
function OnEnter_Tossed_Player(PlayerComponent)
	g_LogManager:Log("Player enters Tossed")
	PlayerComponent:IsBeingTossed(true)
end

function OnUpdate_Tossed_Player(Player, ElapsedTime)
end

function OnExit_Tossed_Player(Player)
	g_LogManager:Log("Player exit Tossed")
	PlayerComponent:IsBeingTossed(false)
end
