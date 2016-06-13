function OnEnter_Idle_RangedEnemy(Enemy)
	g_LogManager:Log(Enemy.m_RObject:GetName().." enters idle")	
end
function OnUpdate_Idle_RangedEnemy(Enemy, ElapsedTime)
	if CTTODMathUtils.PointInsideCircle(g_Player:GetPosition(), Enemy.m_RObject:GetPosition(), Enemy.m_VisionRange) and Enemy:GetTimer() > Enemy:GetAttackDelay()  then 
		Enemy:GetAnimator():SetBool("IsPlayerInsideVisionRange", true)	
	end
end
function OnExit_Idle_RangedEnemy(Enemy)
	Enemy:GetAnimator():SetBool("IsPlayerInsideVisionRange", false)
end

------------------------------------------------------------

function OnEnter_Attack_RangedEnemy(Enemy)
	g_LogManager:Log(Enemy.m_RObject:GetName().." enters attack")
	Enemy:AttachProjectile()
end
function OnUpdate_Attack_RangedEnemy(Enemy, ElapsedTime)
	
end
function OnExit_Attack_RangedEnemy(Enemy)
	--Enemy:LaunchProjectile(g_Player:GetPosition())--PONER POSICION DEL HUESO
end