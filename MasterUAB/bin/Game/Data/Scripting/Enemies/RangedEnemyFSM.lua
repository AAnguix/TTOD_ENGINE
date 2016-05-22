--Idle_State
function OnEnter_Idle_RangedEnemy(Enemy, ElapsedTime)
	Enemy:ResetTime()	
	g_LogManager:Log(Enemy.m_RObject:GetName().." enters idle")	
end

function OnUpdate_Idle_RangedEnemy(Enemy, ElapsedTime)
	local l_AnimatorController = Enemy.m_RObject:GetAnimatorController()
	
	if Enemy:IsPlayerInsideVisionRange(GetPlayer():GetPosition()) and Enemy.m_ElapsedTime > 1  then
		l_AnimatorController:SetBool("IsPlayerInsideVisionRange", true)
	else
		Enemy:AddTime(ElapsedTime)
	end
end

function OnExit_Idle_RangedEnemy(Enemy, ElapsedTime)
	l_AnimatorController:SetBool("IsPlayerInsideVisionRange", false)
end

--Attack_State
function OnEnter_Attack_RangedEnemy(Enemy, ElapsedTime)
	Enemy:ResetTime()	
	g_LogManager:Log(Enemy.m_RObject:GetName().." enters attack")
	--g_PhysXManager:MoveKinematicActor("ball1", l_EnemyPos)
end

function OnUpdate_Attack_RangedEnemy(Enemy, ElapsedTime)
	
end

function OnExit_Attack_RangedEnemy(Enemy, ElapsedTime)
	
end