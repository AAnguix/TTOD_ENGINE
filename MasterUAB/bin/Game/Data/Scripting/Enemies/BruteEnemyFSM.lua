--Idle_State
function OnEnter_Idle_BruteEnemy(Enemy)
	Enemy:ResetTime()	
	g_LogManager:Log(Enemy.m_RObject:GetName().." enters idle")	
end

function OnUpdate_Idle_BruteEnemy(Enemy, ElapsedTime)
	local l_AnimatorController = Enemy.m_RObject:GetAnimatorController()
	
	if Enemy:IsPlayerInsideVisionRange(GetPlayer():GetPosition()) and Enemy.m_ElapsedTime > 3  then
		--if CTTODMathUtils.PointInsideCircle(GetPlayer():GetPosition(), Enemy.m_RObject:GetPosition(), 0.8) then
			l_AnimatorController:SetTrigger("MeleeAttack")
		--else
			--l_AnimatorController:SetTrigger("IsPlayerInsideVisionRange")
		--end
	else
		Enemy:AddTime(ElapsedTime)
	end
	
	if Enemy.m_ElapsedTime > Enemy.m_DelayToPatrol then
		l_AnimatorController:SetBool("DelayToPatrol", true)
	end
		
end

function OnExit_Idle_BruteEnemy(Enemy, ElapsedTime)
	g_LogManager:Log(Enemy.m_RObject:GetName().." exits idle")
	local l_AnimatorController = Enemy.m_RObject:GetAnimatorController()
	l_AnimatorController:SetBool("DelayToPatrol", false)
end


--Charge_State
function OnEnter_Charge_BruteEnemy(Enemy)
	Enemy:ResetTime()
	g_LogManager:Log(Enemy.m_RObject:GetName().." enters Charge")
	Enemy:SetPosititonToAttack(GetPlayer():GetPosition())
end

function OnUpdate_Charge_BruteEnemy(Enemy, ElapsedTime)
	
	Enemy:AddTime(ElapsedTime)
	local l_AnimatorController = Enemy.m_RObject:GetAnimatorController()
	if Enemy:Charge(ElapsedTime) == true then
		l_AnimatorController:SetTrigger("GoBackToIdle")
	end
	
		
	--if Enemy:PointInsideCircle(Enemy:GetPosition(), l_PosToCharge, 0.3) then
	--	g_AnimatorController:SetTrigger("GoBackToIdle")
	--end
	--Enemy:Charge(l_PosToCharge,ElapsedTime)
end

function OnExit_Charge_BruteEnemy(Enemy, ElapsedTime)
	--l_ElapsedTime = 0.0
	--g_AnimatorController:SetBool("IsPlayerInsideVisionRange", false)
	g_LogManager:Log(Enemy.m_RObject:GetName().." exits Charge")
end

--Melee_State
function OnEnter_Melee_BruteEnemy(Enemy)
	Enemy:ResetTime()
	g_LogManager:Log(Enemy.m_RObject:GetName().." enters Melee")
end

function OnUpdate_Melee_BruteEnemy(Enemy, ElapsedTime)
	
end

function OnExit_Melee_BruteEnemy(Enemy, ElapsedTime)	
	g_LogManager:Log(Enemy.m_RObject:GetName().." exits Melee")
end


--Patrol_State
function OnEnter_Patrol_BruteEnemy(Enemy, ElapsedTime)
	Enemy:ResetTime()
	g_LogManager:Log(Enemy.m_RObject:GetName().." enters patrol")
	g_PhysXManager:RemoveTriggerState("ball1")
	local force = Vect3f(0.0,400.0,400.0)
	g_PhysXManager:ApplyForce("Pedestal", force)
	g_PhysXManager:ApplyForce("ball1", force)
end

function OnUpdate_Patrol_BruteEnemy(Enemy, ElapsedTime)
	Enemy:AddTime(ElapsedTime)
	Enemy:FollowTriangleWayPoints(ElapsedTime)
	local l_AnimatorController = Enemy.m_RObject:GetAnimatorController()
	if Enemy:IsPlayerInsideVisionRange(GetPlayer():GetPosition()) then
		l_AnimatorController:SetTrigger("IsPlayerInsideVisionRange")
	end
	
end

function OnExit_Patrol_BruteEnemy(Enemy, ElapsedTime)
	g_LogManager:Log(Enemy.m_RObject:GetName().." exits Patrol")
end

