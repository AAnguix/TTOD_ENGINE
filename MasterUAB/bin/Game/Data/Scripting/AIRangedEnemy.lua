--Idle_State
function OnEnter_Idle_RangedEnemy(RenderableObject, ElapsedTime)
	
end

function OnUpdate_Idle_RangedEnemy(RenderableObject, ElapsedTime)
	if RenderableObject:IsPlayerInsideVisionRange(GetPlayer():GetPosition()) then   
		g_AnimatorController:SetBool("IsPlayerInsideVisionRange", true)  --LLeva a Attack_State
	end
end

function OnExit_Idle_RangedEnemy(RenderableObject, ElapsedTime)
	
end

--Attack_State
function OnEnter_Attack_RangedEnemy(RenderableObject, ElapsedTime)
	local l_PlayerPos = GetPlayer():GetPosition()
	RenderableObject:SetTarget(l_PlayerPos)
end

function OnUpdate_Attack_RangedEnemy(RenderableObject, ElapsedTime)
	l_PlayerPos
	RenderableObject:Attack(l_PlayerPos)
	g_AnimatorController:SetBool("IsPlayerInsideVisionRange", false)  --LLeva a Idle_State
	
	--Discutir si es mejor que el ataque sea solo 1 vez y vuelva a Idle o que se quede aca con un cooldown time o qu hagamos otro chequeo de si esta en el rango de vision
	
end

function OnExit_Attack_RangedEnemy(RenderableObject, ElapsedTime)
	
end