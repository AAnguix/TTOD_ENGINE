function OnEnter_Idle_BruteEnemy(Enemy)
	g_LogManager:Log(Enemy.m_RObject:GetName().." enters idle")	
end
function OnUpdate_Idle_BruteEnemy(Enemy, ElapsedTime)
	if CTTODMathUtils.PointInsideCircle(g_Player:GetPosition(), Enemy.m_RObject:GetPosition(), Enemy.m_VisionRange) and Enemy:GetTimer() > 3.0  then
		--if CTTODMathUtils.PointInsideCircle(g_Player:GetPosition(), Enemy.m_RObject:GetPosition(), 0.8) then
			--Enemy:GetAnimator():SetTrigger("MeleeAttack")
		--else
			--Enemy:GetAnimator():SetTrigger("IsPlayerInsideVisionRange")
		--end
	end
	
	if Enemy:GetTimer() > Enemy.m_DelayToPatrol then
		Enemy:GetAnimator():SetBool("DelayToPatrol", true)
	end
		
end
function OnExit_Idle_BruteEnemy(Enemy)
	g_LogManager:Log(Enemy.m_RObject:GetName().." exits idle")
	Enemy:GetAnimator():SetBool("DelayToPatrol", false)
end

------------------------------------------------------------

function OnEnter_Charge_BruteEnemy(Enemy)
	g_LogManager:Log(Enemy.m_RObject:GetName().." enters Charge")
	Enemy:SetPosititonToAttack(g_Player:GetPosition())
end
function OnUpdate_Charge_BruteEnemy(Enemy, ElapsedTime)
	
	if Enemy:Charge(ElapsedTime) == true then
		Enemy:GetAnimator():SetTrigger("GoBackToIdle")
	end
		
	--if Enemy:PointInsideCircle(Enemy:GetPosition(), l_PosToCharge, 0.3) then
	--	g_AnimatorController:SetTrigger("GoBackToIdle")
	--end
	--Enemy:Charge(l_PosToCharge,ElapsedTime)
end
function OnExit_Charge_BruteEnemy(Enemy)
	--g_AnimatorController:SetBool("IsPlayerInsideVisionRange", false)
	g_LogManager:Log(Enemy.m_RObject:GetName().." exits Charge")
end

------------------------------------------------------------

function OnEnter_Melee_BruteEnemy(Enemy)
	g_LogManager:Log(Enemy.m_RObject:GetName().." enters Melee")
end
function OnUpdate_Melee_BruteEnemy(Enemy, ElapsedTime)
	
end
function OnExit_Melee_BruteEnemy(Enemy)	
	g_LogManager:Log(Enemy.m_RObject:GetName().." exits Melee")
end

------------------------------------------------------------

function OnEnter_Patrol_BruteEnemy(Enemy)
	-- g_LogManager:Log(Enemy.m_RObject:GetName().." enters patrol")
	-- g_PhysXManager:RemoveTriggerState("ball1")
	-- local force = Vect3f(0.0,400.0,400.0)
	-- g_PhysXManager:ApplyForce("Pedestal", force)
	-- g_PhysXManager:ApplyForce("ball1", force)
end
function OnUpdate_Patrol_BruteEnemy(Enemy, ElapsedTime)
	-- Enemy:FollowTriangleWayPoints(ElapsedTime)
	-- if CTTODMathUtils.PointInsideCircle(g_Player:GetPosition(), Enemy.m_RObject:GetPosition(), Enemy.m_VisionRange)  then
		-- Enemy:GetAnimator():SetTrigger("IsPlayerInsideVisionRange")
	-- end
end
function OnExit_Patrol_BruteEnemy(Enemy)
	g_LogManager:Log(Enemy.m_RObject:GetName().." exits Patrol")
end