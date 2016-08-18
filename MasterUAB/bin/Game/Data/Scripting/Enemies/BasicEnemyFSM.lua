BE_StateTotalTime = 0.0
function OnEnter_Idle_BasicEnemy(Enemy)
	g_LogManager:Log(Enemy:GetLuaGameObject():GetName().." enters idle")
	Enemy:SetAttacking(false)
	BE_StateTotalTime = 0.0
end
function OnUpdate_Idle_BasicEnemy(Enemy, ElapsedTime)
	local l_Lgo = Enemy:GetLuaGameObject()
	BE_StateTotalTime = BE_StateTotalTime + ElapsedTime
	local l_PlayerPos = g_Player:GetPosition()
	local l_Pos = l_Lgo:GetPosition()
	if (CTTODMathUtils.PointInsideCircle(l_PlayerPos, l_Pos, Enemy.m_VisionRange) and Enemy:GetTimer() > Enemy:GetAttackDelay())  then 
		if CTTODMathUtils.PointInsideCircle(l_PlayerPos, l_Lgo:GetPosition(), Enemy.m_AttackRange) then
			l_Lgo:SetTrigger("AttackPlayer")
		else
			l_Lgo:SetBool("IsPlayerInsideVisionRange", true) 
		end	
	end
	
	if Enemy:GetTimer() > Enemy.m_DelayToPatrol then
		l_Lgo:SetBool("DelayToPatrol", true)
	end
end
function OnExit_Idle_BasicEnemy(Enemy)
	g_LogManager:Log(Enemy:GetLuaGameObject():GetName().." exits idle")
	--g_LogManager:Log(BE_StateTotalTime.." Tiempo total dentro del estado Idle")
	Enemy:GetLuaGameObject():SetBool("DelayToPatrol", false)
end

------------------------------------------------------------

function OnEnter_Patrol_BasicEnemy(Enemy)
	g_LogManager:Log(Enemy:GetLuaGameObject():GetName().." enters patrol")
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
	g_LogManager:Log(Enemy:GetLuaGameObject():GetName().." exits Patrol")		
end

------------------------------------------------------------

function OnEnter_MoveToAttack_BasicEnemy(Enemy)
	g_LogManager:Log(Enemy:GetLuaGameObject():GetName().." enters MoveToAttack")
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
	g_LogManager:Log(Enemy:GetLuaGameObject():GetName().." exits MoveToAttack")	
end

------------------------------------------------------------

function OnEnter_Attack_BasicEnemy(Enemy)
	g_LogManager:Log(Enemy:GetLuaGameObject():GetName().." enters Attack")
	Enemy:SetAttacking(true)
	BE_StateTotalTime = 0.0
end
function OnUpdate_Attack_BasicEnemy(Enemy, ElapsedTime)
	BE_StateTotalTime = BE_StateTotalTime + ElapsedTime
	if Enemy:IsHit() == true then 
		Enemy:GetLuaGameObject():SetTrigger("GotHit")
	end
end
function OnExit_Attack_BasicEnemy(Enemy)
	g_LogManager:Log(Enemy:GetLuaGameObject():GetName().." exits Attack")
	--g_PlayerComponent:TakeDamage(Enemy:GetWeapon():GetType(),Enemy:GetWeapon():GetDamage())	
	--g_PlayerComponent:TakeDamage("knife",15.0)
	--g_LogManager:Log(BE_StateTotalTime.." Tiempo total dentro del estado Attack")
	Enemy:GetLuaGameObject():SetBool("IsPlayerInsideVisionRange", false)
	g_PlayerComponent:TakeDamage(nil,10)
end


------------------------------------------------------------

function OnEnter_Hit_BasicEnemy(Enemy)
	Enemy:SetHitState(false)
	g_LogManager:Log(Enemy.m_LuaGameObject:GetName().." enters Hit")
	BE_StateTotalTime = 0.0
end
function OnUpdate_Hit_BasicEnemy(Enemy, ElapsedTime)
	BE_StateTotalTime = BE_StateTotalTime + ElapsedTime
end
function OnExit_Hit_BasicEnemy(Enemy)
	g_LogManager:Log(Enemy.m_LuaGameObject:GetName().." exits Hit")
	--g_LogManager:Log(BE_StateTotalTime.." Tiempo total dentro del estado Hit")
end