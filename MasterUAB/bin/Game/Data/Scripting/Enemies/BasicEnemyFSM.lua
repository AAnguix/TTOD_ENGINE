--Idle_State
function OnEnter_Idle_BasicEnemy(Enemy)
	Enemy:ResetTime()
	g_LogManager:Log(Enemy.m_ElapsedTime.." m_ElapsedTime at enters idle and after reset")	
	g_LogManager:Log(Enemy.m_RObject:GetName().." enters idle")	
end

function OnUpdate_Idle_BasicEnemy(Enemy, ElapsedTime)
	local l_AnimatorController = Enemy.m_RObject:GetAnimatorController()

	if CTTODMathUtils.PointInsideCircle(GetPlayer():GetPosition(), Enemy.m_RObject:GetPosition(), Enemy.m_VisionRange) and Enemy.m_ElapsedTime > 1.5  then
		
		if CTTODMathUtils.PointInsideCircle(GetPlayer():GetPosition(), Enemy.m_RObject:GetPosition(), Enemy.m_AttackRange) then
			l_AnimatorController:SetTrigger("AttackPlayer")
		else
			l_AnimatorController:SetBool("IsPlayerInsideVisionRange", true)  --LLeva a MoveToAttack_State
		end	
	else
		Enemy:AddTime(ElapsedTime)
	end
	
	if Enemy.m_ElapsedTime > Enemy.m_DelayToPatrol then
		g_LogManager:Log(Enemy.m_RObject:GetName().." Has enter patrol at time: "..Enemy.m_ElapsedTime)	
		l_AnimatorController:SetBool("DelayToPatrol", true)
	end
end

function OnExit_Idle_BasicEnemy(Enemy)
	g_LogManager:Log(Enemy.m_RObject:GetName().." exits idle")
	local l_AnimatorController = Enemy.m_RObject:GetAnimatorController()
	l_AnimatorController:SetBool("DelayToPatrol", false)
end


--Patrol_State
function OnEnter_Patrol_BasicEnemy(Enemy)
	g_LogManager:Log(Enemy.m_RObject:GetName().." enters Patrol")
	Enemy:ResetTime()
end

function OnUpdate_Patrol_BasicEnemy(Enemy, ElapsedTime)
	Enemy:AddTime(ElapsedTime)
	Enemy:FollowTriangleWayPoints(ElapsedTime)
	local l_AnimatorController = Enemy.m_RObject:GetAnimatorController()
	if CTTODMathUtils.PointInsideCircle(GetPlayer():GetPosition(), Enemy.m_RObject:GetPosition(), Enemy.m_VisionRange) then
		--g_LogManager:Log(Enemy.m_RObject:GetName().." Has seen The Player in patrol state")	
		l_AnimatorController:SetBool("IsPlayerInsideVisionRange", true)
	end
end

function OnExit_Patrol_BasicEnemy(Enemy)
	g_LogManager:Log(Enemy.m_RObject:GetName().." exits Patrol")		
end


--MoveToAttack_State
function OnEnter_MoveToAttack_BasicEnemy(Enemy)
	Enemy:ResetTime()
	g_LogManager:Log(Enemy.m_RObject:GetName().." enters MoveToAttack")
end

function OnUpdate_MoveToAttack_BasicEnemy(Enemy, ElapsedTime)
	Enemy:AddTime(ElapsedTime)
	local l_AnimatorController = Enemy.m_RObject:GetAnimatorController()
	local l_PlayerPos = GetPlayer():GetPosition()
	local l_Pos = Enemy.m_RObject:GetPosition()
	if CTTODMathUtils.PointInsideCircle(l_PlayerPos, l_Pos, Enemy.m_VisionRange) then   
		if CTTODMathUtils.PointInsideCircle(l_PlayerPos, l_Pos, Enemy.m_AttackRange) then
			l_AnimatorController:SetTrigger("AttackPlayer")
		else
			Enemy:MoveToPlayerNearestPoint(l_PlayerPos,ElapsedTime)
		end
	else
		g_LogManager:Log(Enemy.m_RObject:GetName().." Sale de MoveToATTACK porq el player ha salido de vison")
		l_AnimatorController:SetBool("IsPlayerInsideVisionRange", false) --Goes to Idle
	end
end

function OnExit_MoveToAttack_BasicEnemy(Enemy)
	g_LogManager:Log(Enemy.m_RObject:GetName().." exits MoveToAttack")	
end

--Attack_State
function OnEnter_Attack_BasicEnemy(Enemy)
	Enemy:ResetTime()
	g_LogManager:Log(Enemy.m_RObject:GetName().." enters Attack")	
end

function OnUpdate_Attack_BasicEnemy(Enemy, ElapsedTime)
	
end

function OnExit_Attack_BasicEnemy(Enemy)
	g_LogManager:Log(Enemy.m_RObject:GetName().." exits Attack")
	local l_AnimatorController = Enemy.m_RObject:GetAnimatorController()
	l_AnimatorController:SetBool("IsPlayerInsideVisionRange", false) --Goes to Idle
end

