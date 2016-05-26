class 'CDragonComponent' (CLUAComponent)
function CDragonComponent:__init(CRenderableObject, ParticleSystemInstance, BoneID)
	CLUAComponent.__init(self,"dragon")
	self.m_RObject = CRenderableObject
	
	self.m_Dead=false
	self.m_MaxHealth=1000.0
	self.m_Health=740.0
	self.m_Weapons = {}
	self.m_Armor = nil
	self.m_States = {}
	self.m_CurrentState = nil
	self.m_CurrentStateIndex = 0
	
	self.m_AttackDelay = 3.0
	
	self.m_Speed = 1.0
	self.m_Velocity = Vect3f(0.0,0.0,0.0)
	
	self.m_FireParticles = ParticleSystemInstance
	self.m_BoneID = BoneID
	-- self.m_FireParticles:SetParent(CRenderableObject, BoneName)
	self.m_Target = Vect3f(0.0,0.0,0.0)
	
	--Character collider
	self.m_Height = 3.0
	self.m_Radius = 1.5
	self.m_Density = 100
	
	--Components
	self.m_Animator = nil 
	self.m_AudioSource = nil
end

function CDragonComponent:Initialize()
	--Physyx
	local l_CharacterCollider = CCharacterCollider.AddCharacterCollider("CharacterCollider", self.m_RObject)
	if l_CharacterCollider ~= nil then 
		local l_Material = l_CharacterCollider:GetPhysxMaterial()
		local l_MaterialName = l_Material:GetName()
		g_PhysXManager:RegisterMaterial(l_MaterialName, l_Material:GetStaticFriction(), l_Material:GetDynamicFriction(), l_Material:GetRestitution())
		m_Position = self.m_RObject:GetPosition()
		l_CControlerPos = Vect3f(m_Position.x, m_Position.y, m_Position.z)
		g_PhysXManager:CreateCharacterController(self.m_RObject:GetName(), self.m_Height, self.m_Radius , self.m_Density, l_CControlerPos, l_MaterialName)
	end

	self.m_Animator = CAnimatorController.AddAnimatorController("AnimatorController", self.m_RObject)
	local l_Idle = self.m_Animator:AddState("Idle_State", "idle", 1.0, "OnEnter_Idle_Dragon", "OnUpdate_Idle_Dragon", "OnExit_Idle_Dragon")
	local l_Scratch = self.m_Animator:AddState("Scratch_State", "normalAttack", 1.0, "OnEnter_Scratch_Dragon", "OnUpdate_Scratch_Dragon", "OnExit_Scratch_Dragon")
	local l_SpitFire = self.m_Animator:AddState("SpitFire_State", "normalAttack", 1.0, "OnEnter_SpitFire_Dragon", "OnUpdate_SpitFire_Dragon", "OnExit_SpitFire_Dragon")
	local l_Stomp = self.m_Animator:AddState("Stomp_State", "iraAttack", 1.0, "OnEnter_Stomp_Dragon", "OnUpdate_Stomp_Dragon", "OnExit_Stomp_Dragon")
	local l_WhipTale = self.m_Animator:AddState("WhipTale_State", "iraAttack", 1.0, "OnEnter_WhipTale_Dragon", "OnUpdate_WhipTale_Dragon", "OnExit_WhipTale_Dragon")
    
	self.m_Animator:AddBool("IsPlayerInsideScratchRange", false)
	self.m_Animator:AddTrigger("WaitingTimeExpired", false) --The time that the dragon waits beetwen attacks
	self.m_Animator:AddTrigger("TimeSpitingFireExpired", false) --The time that the dragon is spiting fire
	self.m_Animator:AddTrigger("IsHealthBellow25Percent", false)
	self.m_Animator:AddTrigger("IsHealthBellow50Percent", false)
	self.m_Animator:AddTrigger("IsHealthBellow75Percent", false)
	
	self.m_Animator:AddBool("HealthBellow25PercentAttackRaised", false)
	self.m_Animator:AddBool("HealthBellow50PercentAttackRaised", false)
	self.m_Animator:AddBool("HealthBellow75PercentAttackRaised", false)

	local l_IdleToSplitFire = l_Idle:AddTransition("IdleToSplitFire", l_SpitFire, false, 0.0, 0.1, 0.1)
	l_IdleToSplitFire:AddBoolCondition("IsPlayerInsideScratchRange", false)
	l_IdleToSplitFire:AddTriggerCondition("WaitingTimeExpired")
	
	local l_SplitFireToIdle = l_SpitFire:AddTransition("SplitFireToIdle", l_Idle, false, 0.0, 0.1, 0.1)
	l_SplitFireToIdle:AddTriggerCondition("TimeSpitingFireExpired")
	
	local l_IdleToScratch = l_Idle:AddTransition("IdleToScratch", l_Scratch, false, 0.0, 0.1, 0.1)
	l_IdleToScratch:AddBoolCondition("IsPlayerInsideScratchRange", true)
	l_IdleToScratch:AddTriggerCondition("WaitingTimeExpired")
	
	--Special attacks
	local l_IdleToStomp = l_Idle:AddTransition("IdleToStomp", l_Stomp, false, 0.0, 0.1, 0.1)
	l_IdleToStomp:AddTriggerCondition("IsHealthBellow50Percent")
	l_IdleToStomp:AddBoolCondition("HealthBellow50PercentAttackRaised",false)
	
	local l_IdleToWhipTale25 = l_Idle:AddTransition("IdleToWhipTale25", l_WhipTale, false, 0.0, 0.1, 0.1)
	l_IdleToWhipTale25:AddTriggerCondition("IsHealthBellow25Percent")
	l_IdleToWhipTale25:AddBoolCondition("HealthBellow25PercentAttackRaised",false)
	
	local l_IdleToWhipTale75 = l_Idle:AddTransition("IdleToWhipTale75", l_WhipTale, false, 0.0, 0.1, 0.1)
	l_IdleToWhipTale75:AddTriggerCondition("IsHealthBellow75Percent")
	l_IdleToWhipTale75:AddBoolCondition("HealthBellow75PercentAttackRaised",false)
	
	--AttackDelay, ScratchDamage, ScratchRange, SpitFireDamage, TimeSpitingFire, RotationVelocity
	self:AddState(4.0, 10.0, 2.0, 15.0, 3.0, 4.0)
	self:AddState(3.0, 12.0, 2.0, 20.0, 3.0, 4.0)
	self:AddState(2.0, 15.0, 2.0, 30.0, 3.0, 4.0)
	self:AddState(1.0, 20.0, 2.0, 50.0, 3.0, 4.0)
	
	--g_LogManager:Log("Dragon "..self.m_RObject:GetName().." created...")
end

function CDragonComponent:SpitFire()
	--g_LogManager:Log("Escupiendo fuego: "..self.m_FireParticles:GetName())
	self.m_FireParticles:SetVisible(true)
	--g_RenderManager:GetContextManager:SetWorldMatrix(self:GetFireParticlesTransform())
end

function CDragonComponent:GetFireParticlesTransform()
	local l_TransformMatrix = Mat44f()
	local l_DragonTransform = self:GetRenderableObject():GetTransform()
	--local l_HeadBoneTransform = self:GetRenderableObject():GetBoneTransformationMatrix(self.m_BoneID)
	local l_ParticlesLocalTransform = self.m_FireParticles:GetTransform()
	
	l_TransformMatrix = l_DragonTransform * l_ParticlesLocalTransform
	--l_TransformMatrix = l_DragonTransform * l_HeadBoneTransform * l_ParticlesLocalTransform
	--g_LogManager:Log("calculando matriz")
	return l_TransformMatrix
end
function CDragonComponent:StopSpittingFire()
	self.m_FireParticles:SetVisible(false)
end
function CDragonComponent:SetTarget(Target)
	self.m_Target = Target
end

function CDragonComponent:CalculateNewAngle(Angle, CurrentYaw, Velocity, ElapsedTime)
	local l_Result = 0.0
	if Angle < 0.0 then
		if (Velocity*(-1)*ElapsedTime) < Angle then
			l_Result = CurrentYaw + (Angle*ElapsedTime)
		else l_Result = CurrentYaw + (Velocity*(-1)*ElapsedTime)
		end
	else
		if (Velocity*ElapsedTime) > Angle then
			l_Result = CurrentYaw + (Angle*ElapsedTime)
		else l_Result = CurrentYaw + (Velocity*ElapsedTime)
		end
	end
	return l_Result
end

function CDragonComponent:LookAtPoint(Point, ElapsedTime)
	local l_Forward = self.m_RObject:GetForward()
	l_Forward.y = 0.0
	--Point.y = 0.0
	local l_Angle = CTTODMathUtils.GetAngleToFacePoint(l_Forward, self.m_RObject:GetPosition(), Point)	
	-- -k*T    si el ángulo es negativo. K es la velocidad
	--  si -k*T < angulo, aplicas el angulo, y no la formula, para que no se pase de rotación
	local l_CurrentYaw = self.m_RObject:GetYaw()
	local l_Velocity = self.m_CurrentState.m_RotationVelocity

	self.m_RObject:SetYaw(self:CalculateNewAngle(l_Angle, l_CurrentYaw, l_Velocity, ElapsedTime))
end

function CDragonComponent:Update(ElapsedTime)

end

function CDragonComponent:GetRenderableObject() return self.m_RObject end

function CDragonComponent:GetAnimator() return self.m_Animator end
function CDragonComponent:GetAudioSource() return self.m_AudioSource end

function CDragonComponent:IsDead() return self.m_Dead end
function CDragonComponent:GetHealth() return self.m_Health end
function CDragonComponent:GetHealthPercentage()
	return ((self.m_Health*100)/self.m_MaxHealth) 
end
function CDragonComponent:GetSpeed()return self.m_Speed end
function CDragonComponent:GetArmor() return self.m_Armor end
function CDragonComponent:GetRenderableObject() return self.m_RObject end

function CDragonComponent:TakeDamage(PlayerWeapon, PlayerDamage)
	local l_DragonArmor = self.m_Armor:GetType()
	self.m_Health = self.m_Health - (g_DamageCalculator:CalculateDamage(l_DragonArmor,PlayerWeapon,PlayerDamage))
end
function CDragonComponent:NextState()
	if self.m_CurrentStateIndex < #self.m_States then
		self.m_CurrentStateIndex = self.m_CurrentStateIndex+1
		self.m_CurrentState = self.m_States[self.m_CurrentStateIndex]
	else
		self.m_CurrentStateIndex = 1
		self.m_CurrentState = self.m_States[1]
	end
	--g_LogManager:Log("Dragon changes to state "..self.m_CurrentStateIndex)
end
function CDragonComponent:GetCurrentState()
	return self.m_CurrentState
end

-------- DRAGON STATES ------
function CDragonComponent:AddState(AttackDelay, ScratchDamage, ScratchRange, SpitFireDamage, TimeSpitingFire, RotationVelocity)
	local l_DragonState = CDragonState(AttackDelay, ScratchDamage, ScratchRange, SpitFireDamage, TimeSpitingFire, RotationVelocity)
	local l_Size = #self.m_States
	table.insert(self.m_States, l_Size, l_DragonState)
	if self.m_CurrentState == nil then
		--g_LogManager:Log("estado actual es nulo")
		self.m_CurrentState = l_DragonState
	end
	--g_LogManager:Log("estado añadido")
end

class 'CDragonState' 
function CDragonState:__init(AttackDelay, ScratchDamage, ScratchRange, SpitFireDamage, TimeSpitingFire, RotationVelocity)
	self.m_AttackDelay = AttackDelay
	self.m_ScratchDamage = ScratchDamage
	self.m_ScratchRange = ScratchRange
	self.m_SplitFireDamage = SpitFireDamage
	self.m_TimeSpitingFire = TimeSpitingFire
	self.m_RotationVelocity = RotationVelocity
end