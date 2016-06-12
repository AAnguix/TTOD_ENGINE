g_Walk = false
g_Run = false

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
	--g_LogManager:Log("Enters attack")
end

function OnUpdate_Attack_Player(Player, ElapsedTime)
	Player:FaceEnemy(g_GameController:GetEnemies(),ElapsedTime)
end

function OnExit_Attack_Player(Player)
	--g_LogManager:Log("Exit attack")
end

-- function OnEnter_Attack_Player(Player, ElapsedTime)
	
-- end

-- function OnUpdate_Attack_Player(Player, ElapsedTime)
	
	-- local l_PlayerPos = GetPlayer():GetPosition()
	-- if Player:IsPlayerInsideVisionRange(l_PlayerPos) then   
		
		-- if Player:PointInsideCircle(Player:GetPosition(), l_PlayerPos, 0.3) then
			-- g_AnimatorController:SetBool("AttackPlayer",true)
		-- end
		
		-- MoveToPlayerNearestPoint(l_PlayerPos)
	-- else
		-- g_AnimatorController:SetBool("IsPlayerInsideVisionRange", false)  --LLeva a Idle_State
	-- end
	
-- end

-- function OnExit_Attack_Player(Player, ElapsedTime)
	
-- end

--Walk_state
function OnEnter_Walk_Player(Player, ElapsedTime)
	
end

function OnUpdate_Walk_Player(Player, ElapsedTime)
	
end

function OnExit_Walk_Player(Player, ElapsedTime)
	
end

--Block_state
function OnEnter_Block_Player(Player, ElapsedTime)
	
end

function OnUpdate_Block_Player(Player, ElapsedTime)
	
end

function OnExit_Block_Player(Player, ElapsedTime)
	
end

