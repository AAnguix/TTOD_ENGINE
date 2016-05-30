function OnEnter_Idle_Dragon(Dragon, ElapsedTime)
	--g_LogManager:Log(Dragon.m_RObject:GetName().." enters idle")
end
function OnUpdate_Idle_Dragon(Dragon, ElapsedTime)
	
	local l_AttackDelay = Dragon:GetCurrentState().m_AttackDelay
	local l_Animator = Dragon:GetAnimator()
	--g_LogManager:Log("Current attack delay "..l_AttackDelay)
	--g_LogManager:Log("Timer: "..Dragon:GetTimer())
	
	if Dragon:GetHealthPercentage() < 75 then
		--g_LogManager:Log("Health bellow 75%!")
		l_Animator:SetTrigger("IsHealthBellow75Percent")  
	elseif  Dragon:GetHealthPercentage() < 50 then
		--g_LogManager:Log("Health bellow 50%!")
		l_Animator:SetTrigger("IsHealthBellow50Percent")  
	elseif  Dragon:GetHealthPercentage() < 25 then
		--g_LogManager:Log("Health bellow 25%!")
		l_Animator:SetTrigger("IsHealthBellow25Percent")  
	end
	
	if Dragon:GetTimer() > l_AttackDelay then
	
		local l_ScratchRange = Dragon:GetCurrentState().m_ScratchRange
		l_Animator:SetTrigger("WaitingTimeExpired")  
		if CTTODMathUtils.PointInsideCircle(g_Player:GetPosition(), Dragon.m_RObject:GetPosition(), l_ScratchRange)  then 
			l_Animator:SetBool("IsPlayerInsideScratchRange", true) 
		else   
			l_Animator:SetBool("IsPlayerInsideScratchRange", false)  
		end
	end
end
function OnExit_Idle_Dragon(Dragon, ElapsedTime)
end

------------------------------------------------------------

function OnEnter_SpitFire_Dragon(Dragon, ElapsedTime)
	--g_LogManager:Log(Dragon.m_RObject:GetName().." enters SplitFire")
	--Mirar al player, para que cuando lanze la animacion le ataque
end
function OnUpdate_SpitFire_Dragon(Dragon, ElapsedTime)
	Dragon:LookAtPoint(g_Player:GetPosition(),ElapsedTime)
	Dragon:SpitFire()
	
	local l_TimeSpitingFire = Dragon:GetCurrentState().m_TimeSpitingFire
	if Dragon:GetTimer() > l_TimeSpitingFire then
		local l_Animator = Dragon:GetAnimator()
		l_Animator:SetTrigger("TimeSpitingFireExpired")  
	end
end
function OnExit_SpitFire_Dragon(Dragon, ElapsedTime)
 	Dragon:StopSpittingFire()
end

------------------------------------------------------------

function OnEnter_Scratch_Dragon(Dragon, ElapsedTime)
	--g_LogManager:Log(Dragon.m_RObject:GetName().." enters Scratch")
	local l_PlayerPos = g_Player:GetPosition()
	Dragon:SetTarget(l_PlayerPos)
end
function OnUpdate_Scratch_Dragon(Dragon, ElapsedTime)
	Dragon:Attack(l_PlayerPos)
	Dragon:GetAnimator():SetBool("IsPlayerInsideScratchRange", false)  --LLeva a Idle_State
	--Discutir si es mejor que el ataque sea solo 1 vez y vuelva a Idle o que se quede aca con un cooldown time o qu hagamos otro chequeo de si esta en el rango de vision
end
function OnExit_Scratch_Dragon(Dragon, ElapsedTime)
	
end

------------------------------------------------------------

function OnEnter_Stomp_Dragon(Dragon, ElapsedTime)
	--g_LogManager:Log(Dragon.m_RObject:GetName().." did special attack. STOMP")
	local l_PlayerPos = g_Player:GetPosition()
	Dragon:SetTarget(l_PlayerPos)
end
function OnUpdate_Stomp_Dragon(Dragon, ElapsedTime)
	g_AnimatorController:SetBool("IsPlayerInsideScratchRange", false)  --LLeva a Idle_State
	--Discutir si es mejor que el ataque sea solo 1 vez y vuelva a Idle o que se quede aca con un cooldown time o qu hagamos otro chequeo de si esta en el rango de vision
end
function OnExit_Stomp_Dragon(Dragon, ElapsedTime)
	Dragon:GetAnimator():SetBool("HealthBellow50PercentAttackRaised", true)
	Dragon:NextState()
end

------------------------------------------------------------

function OnEnter_WhipTale_Dragon(Dragon, ElapsedTime)
	--g_LogManager:Log(Dragon.m_RObject:GetName().." did special attack. WHIP TALE")
	local l_PlayerPos = g_Player:GetPosition()
	Dragon:SetTarget(l_PlayerPos)
end
function OnUpdate_WhipTale_Dragon(Dragon, ElapsedTime)
	
	Dragon:GetAnimator():SetBool("IsPlayerInsideScratchRange", false)  --LLeva a Idle_State
	--Discutir si es mejor que el ataque sea solo 1 vez y vuelva a Idle o que se quede aca con un cooldown time o qu hagamos otro chequeo de si esta en el rango de vision
end
function OnExit_WhipTale_Dragon(Dragon, ElapsedTime)
	if Dragon:GetHealthPercentage() < 25 then
		Dragon:GetAnimator():SetBool("HealthBellow25PercentAttackRaised", true)
	else
		Dragon:GetAnimator():SetBool("HealthBellow75PercentAttackRaised", true)
	end
	Dragon:NextState()
end