function OnEnter_Idle_Dragon(Dragon, ElapsedTime)
	g_LogManager:Log(Dragon:GetLuaGameObject():GetName().." enters idle")
	g_LogManager:Log("Dragon GetTimer FSM: "..Dragon:GetTimer())
	--Dragon:SpitFire()
	
	if Dragon:GetHealthPercentage() < 25 and Dragon.m_CurrentStateIndex < 3 then
		Dragon:ChangeAttackState(3) 
	elseif  Dragon:GetHealthPercentage() < 50 and Dragon.m_CurrentStateIndex < 2 then
		Dragon:ChangeAttackState(2)  
	elseif  Dragon:GetHealthPercentage() < 75 and Dragon.m_CurrentStateIndex < 1 then
		Dragon:ChangeAttackState(1)   
	end
	
end
function OnUpdate_Idle_Dragon(Dragon, ElapsedTime)
	local l_Lgo = Dragon:GetLuaGameObject()
	--Dragon:LookAtPoint(g_Player:GetPosition(),ElapsedTime) --VA BASTANTE FEO
	
	local l_VectorToPLayer = g_Player:GetPosition() - l_Lgo:GetPosition()
	l_VectorToPLayer:Normalize(1)
	local l_AngleSeparation = CTTODMathUtils.AngleBetweenVectors(l_VectorToPLayer,l_Lgo:GetForward()) 
	g_LogManager:Log("Angle Separation: "..l_AngleSeparation)
	if l_AngleSeparation > 0.78 or l_AngleSeparation < -0.78 then
		l_Lgo:SetBool("Rotate", true)
		g_LogManager:Log("ENTRE AQUI EN EL ANGLE SEPARATION")
	end
	
	local l_AttackDelay = Dragon:GetCurrentState().m_AttackDelay
	
	
	if Dragon:GetTimer() > l_AttackDelay then
		g_LogManager:Log(Dragon:GetLuaGameObject():GetName().." El timepo es mayor del Attack Delay")
		
		local l_ScratchRange = Dragon:GetCurrentState().m_ScratchRange
		
		local l_LeftLegDistance =  (g_Player:GetPosition() - Dragon.mLeftLeg:GetWorldPos()):Length()
		local l_RightLegDistance =  (g_Player:GetPosition() - Dragon.mRightLeg:GetWorldPos()):Length()
		local l_ClosestLeg = nil
		local l_TriggerName = nil
		
		if l_LeftLegDistance < l_RightLegDistance then
			l_ClosestLeg = l_LeftLegDistance
			l_TriggerName = "IsPlayerInsideLeftScratchRange"
		else
			l_ClosestLeg = l_RightLegDistance
			l_TriggerName = "IsPlayerInsideRightScratchRange"
		end
		
		if l_ClosestLeg < l_ScratchRange then
			l_Lgo:SetTrigger(l_TriggerName) 
		else
			l_Lgo:SetTrigger("SpitFire")--ESCUPIR FUEGO
		end
		
	end--END ATTACK_DELAY
end
function OnExit_Idle_Dragon(Dragon, ElapsedTime)
	--Dragon:StopSpittingFire()
end

------------------------------------------------------------

function OnEnter_SpitFire_Dragon(Dragon, ElapsedTime)
	g_LogManager:Log(Dragon:GetLuaGameObject():GetName().." enters SpitFire")
	--Mirar al player, para que cuando lanze la animacion le ataque
	Dragon:SpitFire()
end
function OnUpdate_SpitFire_Dragon(Dragon, ElapsedTime)
	local l_FireCannonLength = 9.0 --PODRIA CAMBIAR DEPENDIENDO DEL ESTADO AL IGUAL QUE EL TAMANO DE LA BOLA Q SE ARROJA DISCUTIR
	local l_Result = SRaycastData()
	local l_EmitterBone =  Dragon.m_ParticleEmitterBone
	local l_EmitterPostion = l_EmitterBone:GetWorldPos()+Vect3f(0.0,0.0,2.0)
	local l_EmitterRotation = l_EmitterBone:GetRotation()
	
	local l_DirectToPlayerVector = g_Player:GetPosition() - l_EmitterPostion
	l_DirectToPlayerVector:Normalize(1)
	local A1 = Vect3f(0.0,0.0,0.0)
	A1 = l_EmitterBone:GetDirectionVector(0)
	local A2 = Vect3f(0.0,0.0,0.0)
	A2 = l_EmitterBone:GetDirectionVector(1)
	local TestDir2 = A1 + A2
	TestDir2:Normalize(1)
	
	local Tren = Vect3f(TestDir2.x,l_DirectToPlayerVector.y,TestDir2.z)
	Tren:Normalize(1)
	
	local GeometryQuery = g_PhysXManager:GeometryQuery("SweepTest",l_EmitterPostion, l_EmitterRotation, Tren, l_FireCannonLength, l_Result)
end
function OnExit_SpitFire_Dragon(Dragon, ElapsedTime)
 	Dragon:StopSpittingFire()
end

------------------------------------------------------------

function OnEnter_ScratchRight_Dragon(Dragon, ElapsedTime)
	g_LogManager:Log(Dragon:GetLuaGameObject():GetName().." Enters Scratch Right")
	Dragon:ChangeAttackingLegsState(true)
end
function OnUpdate_ScratchRight_Dragon(Dragon, ElapsedTime)
	--Dragon:Attack(l_PlayerPos)
	
	--Discutir si es mejor que el ataque sea solo 1 vez y vuelva a Idle o que se quede aca con un cooldown time o qu hagamos otro chequeo de si esta en el rango de vision
end
function OnExit_ScratchRight_Dragon(Dragon, ElapsedTime)
	Dragon:ChangeAttackingLegsState(false)
end

function OnEnter_ScratchLeft_Dragon(Dragon, ElapsedTime)
	g_LogManager:Log(Dragon:GetLuaGameObject():GetName().." Enters Scratch Left")
	Dragon:ChangeAttackingLegsState(true)
end
function OnUpdate_ScratchLeft_Dragon(Dragon, ElapsedTime)
	--Dragon:Attack(l_PlayerPos)
	
	--Discutir si es mejor que el ataque sea solo 1 vez y vuelva a Idle o que se quede aca con un cooldown time o qu hagamos otro chequeo de si esta en el rango de vision
end
function OnExit_ScratchLeft_Dragon(Dragon, ElapsedTime)
	Dragon:ChangeAttackingLegsState(false)
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

--------------------------------------------------------------

function OnEnter_Rotate_Dragon(Dragon, ElapsedTime)
	local l_Lgo = Dragon:GetLuaGameObject()
	g_LogManager:Log("Dragon: ENter Rotate State")
	--g_LogManager:Log("Dragon GetTimer FSM: "..Dragon:GetTimer())
	local l_DirectionToFace = g_Player:GetPosition() - l_Lgo:GetPosition()
	l_DirectionToFace:Normalize(1)
	local l_ForwardBeforeFacing = l_Lgo:GetForward():Normalize(1)
	Dragon:SetAttackFacingValues(l_ForwardBeforeFacing,l_DirectionToFace)
end
function OnUpdate_Rotate_Dragon(Dragon, ElapsedTime)
	local l_Lgo = Dragon:GetLuaGameObject()
	
	if Dragon.m_FacingFinished then
		g_LogManager:Log("Dragon: m_FacingFinished True")
		l_Lgo:SetBool("Rotate", false)
	else
		g_LogManager:Log("Dragon: m_FacingFinished False")
		Dragon:FaceDirection(ElapsedTime)
	end
end
function OnExit_Rotate_Dragon(Dragon, ElapsedTime)
	Dragon.m_FacingFinished = false
end
