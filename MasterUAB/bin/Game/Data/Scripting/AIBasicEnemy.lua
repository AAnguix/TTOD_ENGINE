--Idle_State
function OnEnter_Idle_BasicEnemy(RenderableObject, ElapsedTime)
	
end

function OnUpdate_Idle_BasicEnemy(RenderableObject, ElapsedTime)
	
	if RenderableObject:IsPlayerInsideVisionRange(GetPlayer():GetPosition()) then   
		g_AnimatorController:SetBool("IsPlayerInsideVisionRange", true)  --LLeva a MoveToAttack_State
	else
		l_ElapsedTime += ElapsedTime
	end
	
	if l_ElapsedTime > 1.5 then
		g_AnimatorController:SetBool("DelayToPatrol", true)
	end
	
end

function OnExit_Idle_BasicEnemy(RenderableObject, ElapsedTime)
	
end

--MoveToAttack_State
function OnEnter_MoveToAttack_BasicEnemy(RenderableObject, ElapsedTime)
	
end

function OnUpdate_MoveToAttack_BasicEnemy(RenderableObject, ElapsedTime)
	
	local l_PlayerPos = GetPlayer():GetPosition()
	if RenderableObject:IsPlayerInsideVisionRange(l_PlayerPos) then   
		
		if RenderableObject:PointInsideCircle(RenderableObject:GetPosition(), l_PlayerPos, 0.3) then
			g_AnimatorController:SetBool("AttackPlayer",true)
		end
		
		MoveToPlayerNearestPoint(l_PlayerPos)
	else
		g_AnimatorController:SetBool("IsPlayerInsideVisionRange", false)  --LLeva a Idle_State
	end
	
end

function OnExit_MoveToAttack_BasicEnemy(RenderableObject, ElapsedTime)
	
end

--Attack_State
function OnEnter_Attack_BasicEnemy(RenderableObject, ElapsedTime)
	
end

function OnUpdate_Attack_BasicEnemy(RenderableObject, ElapsedTime)
	
	local l_PlayerPos = GetPlayer():GetPosition()
	if RenderableObject:PointInsideCircle(RenderableObject:GetPosition(), l_PlayerPos, 0.3) then
		g_AnimatorController:SetBool("AttackPlayer",true)
	else
		g_AnimatorController:SetTrigger("GoBackToIdle")
		
	end
	
end

function OnExit_Attack_BasicEnemy(RenderableObject, ElapsedTime)
	g_AnimatorController:SetBool("AttackPlayer",false)
	g_AnimatorController:SetBool("IsPlayerInsideVisionRange", false)
end

--Patrol_State
function OnEnter_Patrol_BasicEnemy(RenderableObject, ElapsedTime)
	local l_PatrolWayPoints = {1, 4, 9}
	RenderableObject:FollowTriangleWayPoints(ElapsedTime, l_PatrolWayPoints)
end

function OnUpdate_Patrol_BasicEnemy(RenderableObject, ElapsedTime)
	if IsPlayerInsideVisionRange(GetPlayer():GetPosition()) then 
		g_AnimatorController:SetBool("IsPlayerInsideVisionRange", true)  
	end
end

function OnExit_Patrol_BasicEnemy(RenderableObject, ElapsedTime)
	
end
