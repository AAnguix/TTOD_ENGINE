g_Walk = false
g_Run = false

--Idle_State
function OnEnter_Idle_Player(Player)
	g_LogManager:Log("Player enters Idle")
end

function OnUpdate_Idle_Player(Player, ElapsedTime)
	
	if (CInputManager.GetInputManager():IsActionActive("PLAYER_WALK_FORWARD")) then
		g_Walk = true
	end
	
	-- if (CInputManager.GetInputManager():IsActionActive("PLAYER_WALK_BACK")) then
		-- Player:GetAnimatorController():SetBool("Walk",false)
	-- end
end

function OnExit_Idle_Player(Player)
	g_LogManager:Log("Player enters Exit")
end

--Attack_State
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
