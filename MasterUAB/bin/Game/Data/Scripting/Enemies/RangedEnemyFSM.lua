function OnEnter_Idle_RangedEnemy(Enemy)
	g_LogManager:Log(Enemy.m_LuaGameObject:GetName().." enters idle")		
end
function OnUpdate_Idle_RangedEnemy(Enemy, ElapsedTime)
	
	local l_Lgo = Enemy:GetLuaGameObject()
	local l_PlayerPos = g_Player:GetPosition()
	local l_Pos = Enemy:GetLuaGameObject():GetPosition()
	
	if CTTODMathUtils.PointInsideCircle(l_PlayerPos, l_Pos, Enemy.m_EscapeRadius)  then 
		l_Lgo:SetTrigger("PlayerInsideEscapeRange") 
	elseif (CTTODMathUtils.PointInsideCircle(l_PlayerPos, l_Pos, Enemy.m_VisionRange) and Enemy:GetTimer() > Enemy:GetAttackDelay())  then 
		l_Lgo:SetTrigger("AttackPlayer")	
	end
end
function OnExit_Idle_RangedEnemy(Enemy)
	--Enemy:GetAnimator():SetBool("IsPlayerInsideVisionRange", false)
	g_LogManager:Log(Enemy.m_LuaGameObject:GetName().." exits idle")
end

------------------------------------------------------------

function OnEnter_Attack_RangedEnemy(Enemy)
	Enemy:SetAttacking(true)
	g_LogManager:Log(Enemy.m_LuaGameObject:GetName().." enters attack")
	Enemy:AttachProjectile()
end
function OnUpdate_Attack_RangedEnemy(Enemy, ElapsedTime)
	local l_PlayerPos = g_Player:GetPosition()
	if ElapsedTime>0.0 then
		Enemy:LookAtPoint(l_PlayerPos,ElapsedTime)
	else 
		Enemy:LookAtPoint(l_PlayerPos,0.0)
	end
end
function OnExit_Attack_RangedEnemy(Enemy)
	g_LogManager:Log(Enemy.m_LuaGameObject:GetName().." exits attack")
	Enemy:LaunchProjectile(g_Player:GetPosition())--PONER POSICION DEL HUESO
	Enemy:SetAttacking(false)
end

-----------------------------------------------------------

function OnEnter_RunAway_RangedEnemy(Enemy)
	
	local l_Lgo = Enemy:GetLuaGameObject()
	Enemy:CalculateJumpDestination()
	
	local l_Distance = (g_Player:GetPosition()-Enemy.m_JumpDestination):Length()
	local l_Random = math.random(6)

	if (l_Random > 3 or l_Distance < Enemy.m_EscapeDistance) and (Enemy.m_LastTimeFrightened==false) then
		l_Lgo:SetTrigger("Frightened")
		Enemy.m_LastTimeFrightened = true
	else 
		Enemy.m_LastTimeFrightened = false
	end
end
function OnUpdate_RunAway_RangedEnemy(Enemy, ElapsedTime)
	
	local l_Lgo = Enemy:GetLuaGameObject()
	local l_Pos = Enemy:GetLuaGameObject():GetPosition()
	
	if CTTODMathUtils.PointInsideCircle(l_Pos, Enemy.m_JumpDestination, 0.2) then
		l_Lgo:SetTrigger("AttackPlayer")
	else
		Enemy:SetMoveToAttackMovement(true)
	end
end

function OnExit_RunAway_RangedEnemy(Enemy)
	Enemy:SetMoveToAttackMovement(false)
end

---------------------------------------------------------------

function OnEnter_Frightened_RangedEnemy(Enemy)
	-- Reproducir sonido miedo
	--Enemy:AttachProjectile()
end
function OnUpdate_Frightened_RangedEnemy(Enemy, ElapsedTime)
	
end
function OnExit_Frightened_RangedEnemy(Enemy)
	--Enemy:LaunchProjectile(g_Player:GetPosition())--PONER POSICION DEL HUESO
end

---------------------------------

function OnEnter_Die_RangedEnemy(Enemy)
	g_LogManager:Log(Enemy.m_LuaGameObject:GetName().." enters die")	
end

function OnUpdate_Die_RangedEnemy(Enemy, ElapsedTime)
end

function OnExit_Die_RangedEnemy(Enemy)
	g_LogManager:Log(Enemy.m_LuaGameObject:GetName().." exits die")	
end
