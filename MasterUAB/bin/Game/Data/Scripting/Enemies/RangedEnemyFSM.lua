function OnEnter_Idle_RangedEnemy(Enemy, ElapsedTime)
	g_LogManager:Log(Enemy.m_RObject:GetName().." enters idle")	
end
function OnUpdate_Idle_RangedEnemy(Enemy, ElapsedTime)
	if Enemy:IsPlayerInsideVisionRange(g_Player:GetPosition()) and Enemy.m_ElapsedTime > 1  then
		Enemy:GetAnimator():SetBool("IsPlayerInsideVisionRange", true)
	end
end
function OnExit_Idle_RangedEnemy(Enemy, ElapsedTime)
	Enemy:GetAnimator():SetBool("IsPlayerInsideVisionRange", false)
end

------------------------------------------------------------

function OnEnter_Attack_RangedEnemy(Enemy, ElapsedTime)
	g_LogManager:Log(Enemy.m_RObject:GetName().." enters attack")
end
function OnUpdate_Attack_RangedEnemy(Enemy, ElapsedTime)
	
end
function OnExit_Attack_RangedEnemy(Enemy, ElapsedTime)
	
end