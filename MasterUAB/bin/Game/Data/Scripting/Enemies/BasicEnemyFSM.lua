-------------------- IDLE --------------------

function OnEnter_Idle_BasicEnemy(Enemy)
end
function OnUpdate_Idle_BasicEnemy(Enemy, ElapsedTime)
	if Enemy:GetTimer() > Enemy.m_DelayToPatrol then
		Enemy:GetLuaGameObject():SetBool("DelayToPatrol", true)
	end
end
function OnExit_Idle_BasicEnemy(Enemy)
	--g_LogManager:Log(BE_StateTotalTime.." Tiempo total dentro del estado Idle")
	Enemy:GetLuaGameObject():SetBool("DelayToPatrol", false)
end

-------------------- COMBAT_IDLE --------------------

function OnEnter_CombatIdle_BasicEnemy(Enemy)
	Enemy:SetAttacking(false)
end
function OnUpdate_CombatIdle_BasicEnemy(Enemy, ElapsedTime)
	local l_Lgo = Enemy:GetLuaGameObject()
	local l_PlayerPos = g_Player:GetPosition()
	local l_Pos = l_Lgo:GetPosition()
	Enemy:LookAtPoint(l_PlayerPos, ElapsedTime)
	if (CTTODMathUtils.PointInsideCircle(l_PlayerPos, l_Pos, Enemy.m_VisionRange)) then
		l_Lgo:SetBool("IsPlayerInsideVisionRange", true) 
		
		if (Enemy:GetTimer() > Enemy:GetAttackDelay()) and (CTTODMathUtils.PointInsideCircle(l_PlayerPos, l_Lgo:GetPosition(), Enemy.m_AttackRange)) then
			l_Lgo:SetTrigger("AttackPlayer")
		end	
	else 
		l_Lgo:SetBool("IsPlayerInsideVisionRange", false)
	end
end
function OnExit_CombatIdle_BasicEnemy(Enemy)
end

-------------------- PATROL --------------------

function OnEnter_Patrol_BasicEnemy(Enemy)
	Enemy:SetFollowTriangleWayPointsState(true)
end
function OnUpdate_Patrol_BasicEnemy(Enemy, ElapsedTime)
	--Enemy:FollowTriangleWayPoints(ElapsedTime)
	if CTTODMathUtils.PointInsideCircle(g_Player:GetPosition(), Enemy:GetLuaGameObject():GetPosition(), Enemy.m_VisionRange) then
		Enemy:GetLuaGameObject():SetBool("IsPlayerInsideVisionRange", true)
	end
	--g_LogManager:Log("Patroling...")
end
function OnExit_Patrol_BasicEnemy(Enemy)
	Enemy:SetFollowTriangleWayPointsState(false)
end

-------------------- MOVE_TO_ATTACK -------------------- 

function OnEnter_MoveToAttack_BasicEnemy(Enemy)
end
function OnUpdate_MoveToAttack_BasicEnemy(Enemy, ElapsedTime)
	local l_PlayerPos = g_Player:GetPosition()
	local l_Pos = Enemy:GetLuaGameObject():GetPosition()
	if CTTODMathUtils.PointInsideCircle(l_PlayerPos, l_Pos, Enemy.m_VisionRange) then  
		if CTTODMathUtils.PointInsideCircle(l_PlayerPos, l_Pos, Enemy.m_AttackRange) then
			Enemy:GetLuaGameObject():SetTrigger("AttackPlayer")
		else
			Enemy:SetMoveToAttackMovement(true)
		end
	else
		Enemy:GetLuaGameObject():SetBool("IsPlayerInsideVisionRange", false)
	end
end
function OnExit_MoveToAttack_BasicEnemy(Enemy)
	 Enemy:SetMoveToAttackMovement(false)
end

-------------------- ATTACK -------------------- 

function OnEnter_Attack_BasicEnemy(Enemy)
	Enemy:SetAttacking(true)
end
function OnUpdate_Attack_BasicEnemy(Enemy, ElapsedTime)
	local l_PlayerPos = g_Player:GetPosition()
	local l_Pos = Enemy:GetLuaGameObject():GetPosition()
	if CTTODMathUtils.PointInsideCircle(l_PlayerPos, l_Pos, Enemy.m_AttackStepBackDistance) then  
		local l_EnemyForward = Enemy.m_LuaGameObject:GetForward()    
		Enemy.m_Velocity = Enemy.m_Velocity + (l_EnemyForward*(-1.0)*Enemy.m_AttackStepBackDisplacement)    
	end
end
function OnExit_Attack_BasicEnemy(Enemy)
	Enemy:GetLuaGameObject():SetBool("IsPlayerInsideVisionRange", false)
end

-------------------- HIT -------------------- 

function OnEnter_Hit_BasicEnemy(Enemy)
	--Enemy:SetHitState(false)
end
function OnUpdate_Hit_BasicEnemy(Enemy, ElapsedTime)
	local l_EnemyForward = Enemy.m_LuaGameObject:GetForward()    
	Enemy.m_Velocity = Enemy.m_Velocity + ((l_EnemyForward*(-1.0))*Enemy.m_HitDisplacement)
end
function OnExit_Hit_BasicEnemy(Enemy)
	--Enemy:SetHitState(false)
end

-------------------- DEAD -------------------- 

function OnEnter_Dead_BasicEnemy(Enemy)
end
function OnUpdate_Dead_BasicEnemy(Enemy, ElapsedTime)
end
function OnExit_Dead_BasicEnemy(Enemy)
end