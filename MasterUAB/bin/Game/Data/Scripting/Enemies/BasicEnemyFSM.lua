function OnEnter_Idle_BasicEnemy(Enemy)
	-- g_LogManager:Log(Enemy.m_RObject:GetName().." enters idle")	
end
function OnUpdate_Idle_BasicEnemy(Enemy, ElapsedTime)
	-- if CTTODMathUtils.PointInsideCircle(g_Player:GetPosition(), Enemy.m_RObject:GetPosition(), Enemy.m_VisionRange) and Enemy:GetTimer() > Enemy:GetAttackDelay()  then 
		-- if CTTODMathUtils.PointInsideCircle(g_Player:GetPosition(), Enemy.m_RObject:GetPosition(), Enemy.m_AttackRange) then
			-- Enemy:GetAnimator():SetTrigger("AttackPlayer")
		-- else
			-- Enemy:GetAnimator():SetBool("IsPlayerInsideVisionRange", true) 
		-- end	
	-- end
	
	-- if Enemy:GetTimer() > Enemy.m_DelayToPatrol then
		-- Enemy:GetAnimator():SetBool("DelayToPatrol", true)
	-- end
end
function OnExit_Idle_BasicEnemy(Enemy)
	-- g_LogManager:Log(Enemy.m_RObject:GetName().." exits idle")
	Enemy:GetAnimator():SetBool("DelayToPatrol", false)
end

------------------------------------------------------------

function OnEnter_Patrol_BasicEnemy(Enemy)
	-- g_LogManager:Log(Enemy.m_RObject:GetName().." enters Patrol")
end
function OnUpdate_Patrol_BasicEnemy(Enemy, ElapsedTime)
	Enemy:FollowTriangleWayPoints(ElapsedTime)
	if CTTODMathUtils.PointInsideCircle(g_Player:GetPosition(), Enemy.m_RObject:GetPosition(), Enemy.m_VisionRange) then
		Enemy:GetAnimator():SetBool("IsPlayerInsideVisionRange", true)
	end
end
function OnExit_Patrol_BasicEnemy(Enemy)
	-- g_LogManager:Log(Enemy.m_RObject:GetName().." exits Patrol")		
end

------------------------------------------------------------

function OnEnter_MoveToAttack_BasicEnemy(Enemy)
	-- g_LogManager:Log(Enemy.m_RObject:GetName().." enters MoveToAttack")
end
function OnUpdate_MoveToAttack_BasicEnemy(Enemy, ElapsedTime)
	local l_PlayerPos = g_Player:GetPosition()
	local l_Pos = Enemy.m_RObject:GetPosition()
	if CTTODMathUtils.PointInsideCircle(l_PlayerPos, l_Pos, Enemy.m_VisionRange) then  
		if CTTODMathUtils.PointInsideCircle(l_PlayerPos, l_Pos, Enemy.m_AttackRange) then
			Enemy:GetAnimator():SetTrigger("AttackPlayer")
		else
			Enemy:MoveToPlayerNearestPoint(l_PlayerPos,ElapsedTime)
		end
	else
		Enemy:GetAnimator():SetBool("IsPlayerInsideVisionRange", false)
	end
end
function OnExit_MoveToAttack_BasicEnemy(Enemy)
	-- g_LogManager:Log(Enemy.m_RObject:GetName().." exits MoveToAttack")	
end

------------------------------------------------------------

function OnEnter_Attack_BasicEnemy(Enemy)
	-- g_LogManager:Log(Enemy.m_RObject:GetName().." enters Attack")
end
function OnUpdate_Attack_BasicEnemy(Enemy, ElapsedTime)
	
end
function OnExit_Attack_BasicEnemy(Enemy)
	-- g_LogManager:Log(Enemy.m_RObject:GetName().." exits Attack")
	--g_PlayerComponent:TakeDamage(Enemy:GetWeapon():GetType(),Enemy:GetWeapon():GetDamage())	
	g_PlayerComponent:TakeDamage("knife",15.0)	
	Enemy:GetAnimator():SetBool("IsPlayerInsideVisionRange", false)
end