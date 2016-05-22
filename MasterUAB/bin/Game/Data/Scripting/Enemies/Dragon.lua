class 'CDragonComponent' (CLUAComponent)
function CDragonComponent:__init(CRenderableObject, ComponentType)
	CLUAComponent.__init(self,ComponentType)
	self.m_RObject = CRenderableObject
	
	self.m_Dead=false
	self.m_Health=1000.0
	self.m_Attacks = {}
	self.m_Armor = nil
	
	self.m_Speed = 1.0
	self.m_Velocity = Vect3f(0.0,0.0,0.0)
	self.m_VisionRange = 0.0
	self.m_RotationVelocity = 5.0
	self.m_ElapsedTime = 0.0
	
	self.m_Height = 3.0
	self.m_Radius = 1.5
	self.m_Density = 100
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
	--Animations
	local l_AnimatorController = CAnimatorController.AddAnimatorController("AnimatorController", self.m_RObject)
	
	local l_Idle = l_AnimatorController:AddState("Idle_State", "idle", 1.0, "OnEnter_Idle_BasicEnemy", "OnUpdate_Idle_BasicEnemy", "OnExit_Idle_BasicEnemy")
	local l_Scratch = l_AnimatorController:AddState("Scratch_State", "scratch", 1.0, "OnEnter_MoveToAttack_BasicEnemy", "OnUpdate_MoveToAttack_BasicEnemy", "OnExit_MoveToAttack_BasicEnemy")
	local l_Stomp = l_AnimatorController:AddState("Stomp_State", "stomp", 1.0, "OnEnter_Attack_BasicEnemy", "OnUpdate_Attack_BasicEnemy", "OnExit_Attack_BasicEnemy")
	local l_WhipTale = l_AnimatorController:AddState("WhipTale_State", "whiptale", 1.0, "OnEnter_Patrol_BasicEnemy", "OnUpdate_Patrol_BasicEnemy", "OnExit_Patrol_BasicEnemy")
         
                                          
	l_AnimatorController:AddBool("IsPlayerInsideVisionRange", false)
	l_AnimatorController:AddBool("DelayToPatrol", false)
	l_AnimatorController:AddTrigger("AtackPlayer", false)

	local l_IdleToPatrol = l_Idle:AddTransition("IdleToPatrol", l_Patrol, false, 0.0, 0.1, 0.1)
	l_IdleToPatrol:AddBoolCondition("DelayToPatrol", true)
	l_IdleToPatrol:AddBoolCondition("IsPlayerInsideVisionRange", false)
	
	local l_IdleToMoveToAttack = l_Idle:AddTransition("IdleToMoveToAttack", l_MoveToAttack, false, 0.0, 0.1, 0.1)
	l_IdleToMoveToAttack:AddBoolCondition("IsPlayerInsideVisionRange", true)
	
	local l_PatrolToMoveToAttack = l_Patrol:AddTransition("PatrolToMoveToAttack", l_MoveToAttack, false,0.0, 0.1, 0.1)
	l_PatrolToMoveToAttack:AddBoolCondition("IsPlayerInsideVisionRange", true)
	
	local l_MoveToAttackToAttack = l_MoveToAttack:AddTransition("MoveToAttackToAttack", l_Attack, false, 0.0, 0.1, 0.1)
	l_MoveToAttackToAttack:AddTriggerCondition("AtackPlayer")	
	
	
	g_LogManager:Log("Dragon "..self.m_RObject:GetName().." created...")
end

function CDragonComponent:Update(ElapsedTime)
end

function CDragonComponent:ResetTime()
	self.m_ElapsedTime=0.0
end

function CDragonComponent:AddTime(ElapsedTime)
	self.m_ElapsedTime = self.m_ElapsedTime+ElapsedTime
end

function CDragonComponent:IsDead()
	return self.m_Dead
end
function CDragonComponent:GetHealth()
	return self.m_Health
end
function CDragonComponent:GetSpeed()
	return self.m_Speed
end
function CDragonComponent:GetArmor()
	return self.m_Armor 
end
function CDragonComponent:GetRenderableObject()
	return self.m_RObject
end

function CDragonComponent:TakeDamage(PlayerWeapon, PlayerDamage)
	local l_DragonArmor = self.m_Armor:GetType()
	self.m_Health = self.m_Health - (g_DamageCalculator:CalculateDamage(l_DragonArmor,PlayerWeapon,PlayerDamage))
end

function CDragonComponent:IsPlayerInsideVisionRange(PlayerPosition)

	local l_Distance = 0.0
	local l_Position = self.m_RObject:GetPosition()
	
	l_Distance = math.sqrt((l_Position.x - PlayerPosition.x)^2 + (l_Position.y - PlayerPosition.y)^2 + (l_Position.z - PlayerPosition.z)^2)
	
	-- local s = ""
	-- s = l_Distance.." distance"
	-- g_LogManager:Log(s)
	
	if l_Distance < self.m_VisionRange then
		return true
	end
	
	return false
end



