g_Walk = false
g_Run = false


--[[class 'CLUAComponent' 
	function CLUAComponent:__init()
		self.m_Type=""
	end
	function CLUAComponent:Update()
	end
	function CLUAComponent:Render()
	end
	function CLUAComponent:Init()
	end
	function CLUAComponent:GetType()
		return self.m_Type
	end
--end]]

-- class 'CPlayerComponent' (CLUAComponent)
	-- function CPlayerComponent:__init()
		-- self.m_Typer="CPlayerComponent"
	-- end
	-- function CPlayerComponent:Update()
	-- end
	-- function CPlayerComponent:Render()
	-- end
	-- function CPlayerComponent:Init()
	-- end

-- class 'CEnemyComponent' (CLUAComponent)
	-- function CEnemyComponent:__init()
	-- end
	-- function CEnemyComponent:Update()
	-- end
	-- function CEnemyComponent:Render()
	-- end
	-- function CEnemyComponent:Init()
	-- end
	-- function CEnemyComponent:OnEnter()
	-- end

-- en C++
	-- Tienes un vector<CLUAComponent>
	-- En renderableObject haces un AddComponent
	
-- function CreatePlayer()
	-- l_Player=CreatePlayerCompnent()
	-- RenderableObject:AddComponent(l_Player)
-- end

-- function OnUpdateComponent(Component)
	-- Component:Update()
-- end

-- function OnRenderComponent(Component)
	-- Component:Render()
-- end

-- function OnInitComponent(Component)
	-- Component:Init()
-- end

--Idle_State
function OnEnter_Idle_Player(Player)
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
function OnEnter_Attack_Player(Player, ElapsedTime)
	--g_LogManager:Log("Enters attack")
end

function OnUpdate_Attack_Player(Player, ElapsedTime)
	
end

function OnExit_Attack_Player(Player, ElapsedTime)
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

