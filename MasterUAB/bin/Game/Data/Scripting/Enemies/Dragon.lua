class 'CDragonComponent' (CLUAComponent)
function CDragonComponent:__init(CLuaGameObject, BoneID)--(CLuaGameObject, ParticleSystemInstance, BoneID)
	CLUAComponent.__init(self,CLuaGameObject:GetName().."_script")
	self.m_LuaGameObject = CLuaGameObject
	
	self.m_MaxHealth=1000.0
	self.m_Health=740.0
	self.m_Weapons = {}
	self.m_Armor = nil
	self.m_States = {}
	self.m_CurrentState = nil
	self.m_CurrentStateIndex = 0
	
	self.m_AttackDelay = 3.0
	self.m_TimeStuned = 0.0
	
	self.m_Speed = 1.0
	self.m_Velocity = Vect3f(0.0,0.0,0.0)
	
	--self.m_FireParticles = ParticleSystemInstance
	self.m_BoneID = BoneID
	-- self.m_FireParticles:SetParent(CLuaGameObject, BoneName)
	self.m_Target = Vect3f(0.0,0.0,0.0)
	
	self.m_PhysicsTail = {}
	self.m_PhysicsBody = {}
	self.m_PhysicsHead = {}
	--Character collider
	self.m_Height = 3.0
	self.m_Radius = 2.0
	self.m_Density = 100
	
	self.m_CountdownToExtintionTimer = 8.0
	self.m_CountdownToExtintion = self.m_CountdownToExtintionTimer
	
end

function CDragonComponent:Initialize()
	
	--local l_GameObject = self.m_LuaGameObject:GetGameObject()
	local l_CColliderName = self.m_LuaGameObject:GetName().."_CharacterCollider"
	self:CreateSkeleton()
	--g_PhysXManager:AddCharacterColliderComponent(l_CColliderName, l_GameObject, self.m_Height, self.m_Radius, self.m_Density)
	
	l_ACName = self.m_LuaGameObject:GetName().."_AnimatorController"
	g_AnimatorControllerManager:AddComponent(l_ACName, self.m_LuaGameObject)
	
	-- if l_CharacterCollider ~= nil then 
		-- local l_Material = l_CharacterCollider:GetPhysxMaterial()
		-- local l_MaterialName = l_Material:GetName()
		-- g_PhysXManager:RegisterMaterial(l_MaterialName, l_Material:GetStaticFriction(), l_Material:GetDynamicFriction(), l_Material:GetRestitution())
		-- m_Position = self.m_RObject:GetPosition()
		-- l_CControlerPos = Vect3f(m_Position.x, m_Position.y, m_Position.z)
		-- g_PhysXManager:CreateCharacterController(self.m_GameObject:GetName(), self.m_Height, self.m_Radius , self.m_Density, l_CControlerPos, l_MaterialName)
	-- end

	--self.m_Animator = CAnimatorController.AddAnimatorController("AnimatorController", self.m_GameObject)
	local l_Idle = self.m_LuaGameObject:AddState("Idle_State", "idle", 1.0, "OnEnter_Idle_Dragon", "OnUpdate_Idle_Dragon", "OnExit_Idle_Dragon")
	local l_Scratch = self.m_LuaGameObject:AddState("Scratch_State", "slash", 1.0, "OnEnter_Scratch_Dragon", "OnUpdate_Scratch_Dragon", "OnExit_Scratch_Dragon")
	--local l_SpitFire = self.m_LuaGameObject:AddState("SpitFire_State", "normalAttack", 1.0, "OnEnter_SpitFire_Dragon", "OnUpdate_SpitFire_Dragon", "OnExit_SpitFire_Dragon")
	--local l_Stomp = self.m_LuaGameObject:AddState("Stomp_State", "iraAttack", 1.0, "OnEnter_Stomp_Dragon", "OnUpdate_Stomp_Dragon", "OnExit_Stomp_Dragon")
	--local l_WhipTale = self.m_LuaGameObject:AddState("WhipTale_State", "iraAttack", 1.0, "OnEnter_WhipTale_Dragon", "OnUpdate_WhipTale_Dragon", "OnExit_WhipTale_Dragon")
    
	self.m_LuaGameObject:AddBool("IsPlayerInsideScratchRange", false)
	self.m_LuaGameObject:AddTrigger("WaitingTimeExpired", false) --The time that the dragon waits beetwen attacks
	self.m_LuaGameObject:AddTrigger("TimeSpitingFireExpired", false) --The time that the dragon is spiting fire
	self.m_LuaGameObject:AddTrigger("IsHealthBellow25Percent", false)
	self.m_LuaGameObject:AddTrigger("IsHealthBellow50Percent", false)
	self.m_LuaGameObject:AddTrigger("IsHealthBellow75Percent", false)

	self.m_LuaGameObject:AddBool("HealthBellow25PercentAttackRaised", false)
	self.m_LuaGameObject:AddBool("HealthBellow50PercentAttackRaised", false)
	self.m_LuaGameObject:AddBool("HealthBellow75PercentAttackRaised", false)

	--local l_IdleToSplitFire = l_Idle:AddTransition("IdleToSplitFire", l_SpitFire, false, 0.1, 0.1)
	--l_IdleToSplitFire:AddBoolCondition("IsPlayerInsideScratchRange", false)
	--l_IdleToSplitFire:AddTriggerCondition("WaitingTimeExpired")
	
	--local l_SplitFireToIdle = l_SpitFire:AddTransition("SplitFireToIdle", l_Idle, false, 0.1)
	--l_SplitFireToIdle:AddTriggerCondition("TimeSpitingFireExpired")
	
	local l_IdleToScratch = l_Idle:AddTransition("IdleToScratch", l_Scratch, false, 0.2, 0.2)
	l_IdleToScratch:AddBoolCondition("IsPlayerInsideScratchRange", true)
	l_IdleToScratch:AddTriggerCondition("WaitingTimeExpired")
	
	
	local l_ScratchToIdle = l_Scratch:AddTransition("ScratchToIdle", l_Idle, true, 0.3)

	--Special attacks
	--local l_IdleToStomp = l_Idle:AddTransition("IdleToStomp", l_Stomp, false, 0.1, 0.1)
	--l_IdleToStomp:AddTriggerCondition("IsHealthBellow50Percent")
	--l_IdleToStomp:AddBoolCondition("HealthBellow50PercentAttackRaised",false)
	
	--local l_IdleToWhipTale25 = l_Idle:AddTransition("IdleToWhipTale25", l_WhipTale, false, 0.1, 0.1)
	--l_IdleToWhipTale25:AddTriggerCondition("IsHealthBellow25Percent")
	--l_IdleToWhipTale25:AddBoolCondition("HealthBellow25PercentAttackRaised",false)
	
	--local l_IdleToWhipTale75 = l_Idle:AddTransition("IdleToWhipTale75", l_WhipTale, false, 0.1, 0.1)
	--l_IdleToWhipTale75:AddTriggerCondition("IsHealthBellow75Percent")
	--l_IdleToWhipTale75:AddBoolCondition("HealthBellow75PercentAttackRaised",false)
	
	--AttackDelay, ScratchDamage, ScratchRange, SpitFireDamage, TimeSpitingFire, RotationVelocity
	self:AddState(4.0, 10.0, 14.0, 15.0, 3.0, 4.0)
	self:AddState(3.0, 12.0, 2.0, 20.0, 3.0, 4.0)
	self:AddState(2.0, 15.0, 2.0, 30.0, 3.0, 4.0)
	self:AddState(1.0, 20.0, 2.0, 50.0, 3.0, 4.0)
	
	g_LogManager:Log("Dragon created...")
	
	
end

function CDragonComponent:Die()
	self.m_Dead = true
end

function CDragonComponent:Update(ElapsedTime)
	if (self.m_Dead) then
		self:CountdownToExtintion(ElapsedTime)
	else
		local l_Stuned = self:CheckStuned()
		self:UpdatePxSkeleton()
	end
end

function CDragonComponent:CountdownToExtintion(ElapsedTime)
	self.m_CountdownToExtintionTimer = self.m_CountdownToExtintionTimer - ElapsedTime
	
	if(self.m_CountdownToExtintionTimer <= 0.0) then
		g_GameController:RemoveElementFromTable(g_GameController:GetEnemies(),self.m_LuaGameObject:GetName())
	end
end

dofile("./Data/Scripting/Enemies/DragonSkeleton.lua")

function CDragonComponent:ChangeTailState(State)
	if  #self.m_PhysicsTail > 0 then
		for i, value in ipairs(self.m_PhysicsTail) do
			self.m_PhysicsTail[i]:ChangeTriggerState(State)
		end
	end
end

function CDragonComponent:GetLuaGameObject() return self.m_LuaGameObject end
--function CDragonComponent:GetSpeed()	return self.m_Speed end
function CDragonComponent:GetSpeed()return self.m_Speed end
function CDragonComponent:GetArmor() return self.m_Armor end
function CDragonComponent:GetHealth() return self.m_Health end
function CDragonComponent:StopSpittingFire() self.m_FireParticles:SetVisible(false) end
function CDragonComponent:SetTarget(Target) self.m_Target = Target end


function CDragonComponent:OnTriggerEnter(Actor)
	if Actor == "MagicBall" then
		self:TakeMagicDamage(g_MagicBall:GetDamage())
		self:Stun(g_MagicBall:GetStunTime())
	end	
end

function CDragonComponent:OnTriggerExit(Actor)
end

function CDragonComponent:CreatePhysxSqueleton()
	
	local l_StaticFriction = 1.0
	local l_DynamicFriction = 1.0
	local l_Restitution = 0.5
	local l_MaterialName = "Dragon_tail_material"
	
	local l_Name = 	self.m_GameObject:GetName()
	local l_Size = Vect3f(3.0,1.0,1.0)
	local l_Position = Vect3f(0.0,0.0,0.0)
	local l_Orientation = Quatf(0.0,0.0,0.0,1.0)
	local l_Group = 2
	local l_Density = 5.0
	
	local l_StaticFriction = 1.0
	local l_DynamicFriction = 1.0
	local l_Restitution = 1.0
	
	g_PhysXManager:CreateBox(l_Name.."_tail", l_Size,  l_MaterialName, l_StaticFriction, l_DynamicFriction, l_Restitution, l_Group, true)
	g_PhysXManager:CreateDynamicActor(l_Name.."_tail", l_Name.."_tail", l_Position, l_Orientation, l_Density, true)
	
	g_LogManager:Log("Dragon physx squeleton generated...")
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


function CDragonComponent:CheckStuned(ElapsedTime)
	if self.m_TimeStuned>0 then
		self.m_TimeStuned = self.m_TimeStuned - ElapsedTime
		return true
	end
	return false
end

function CDragonComponent:GetHealthPercentage()
	return ((self.m_Health*100)/self.m_MaxHealth) 
end


function CDragonComponent:TakeDamage(PlayerWeapon, PlayerDamage)
	local l_DragonArmor = self.m_Armor:GetType()
	local l_DamageCalculated = g_DamageCalculator:CalculateDamage(l_DragonArmor,PlayerWeapon,PlayerDamage)
	self:TakeSimpleDamage(l_DamageCalculated)
end

function CDragonComponent:TakeSimpleDamage(Damage)
	if self.m_Health > 0.0 then
		if((self.m_Health - Damage)<=0.0) then
			self.m_Health = 0.0
			self:Die()
		else
			self.m_Health = self.m_Health - Damage
		end
	end
end

function CDragonComponent:Stun(Seconds)
	self.m_TimeStuned = Seconds
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