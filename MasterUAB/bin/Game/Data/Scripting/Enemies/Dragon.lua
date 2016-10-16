class 'CDragonComponent' (CLUAComponent)
function CDragonComponent:__init(CLuaGameObject, BoneName)--(CLuaGameObject, ParticleSystemInstance, BoneID)
	CLUAComponent.__init(self,CLuaGameObject:GetName().."_script")
	self.m_LuaGameObject = CLuaGameObject
	self.m_CoreModel = CLuaGameObject:GetAnimatedCoreModel()
		
	
	self.m_MaxHealth=1000.0
	self.m_Health=300.0
	self.m_Weapons = {}
	self.m_Armor = nil
	
	--States
	self.m_States = {}
	self.m_CurrentState = nil
	self.m_CurrentStateIndex = 0
	
	self.m_AttackDelay = 3.0
	self.m_TimeStuned = 0.0
	
	self.m_Speed = 1.0
	self.m_Velocity = Vect3f(0.0,0.0,0.0)
	
	self.m_ParticleEmitterBoneName = BoneName
	self.mFireParticlesGameObjectName = "smoke001"
	self.m_FireParticles = g_GameController:AddLuaGameObjectHandle(self.mFireParticlesGameObjectName)
	
	
	--Bones
	self.m_ParticleEmitterBone = nil
	self.mLeftLeg = nil
	self.mRightLeg = nil 
	self.m_Target = Vect3f(0.0,0.0,0.0)
	self.m_PhysicsTail = {}
	self.m_PhysicsBody = {}
	self.m_PhysicsHead = {}
	
	--Facing direcition variables
	self.m_AngleMargin = 0.15
	self.m_RotationAngle = 0.0
	self.m_RotationDuration = 0.0
	self.m_YawBeforeFacing = 0.0
	self.m_YawAfterFacing = 0.0
	self.m_FacingFinished = false
	
	--Shape for the Sweep 
	g_PhysXManager:CreateSphereTrigger("SweepTest","SweepTest",1.0,"SweepTest_Material","",Vect3f(0.0,15.0,0.0),Quatf(0.0,0.0,0.0,1.0),"kinematic")
end

function CDragonComponent:Initialize()
	self:CreateSkeleton()
	self.m_FireParticles:EnableRenderableObject(false)
	self.m_ParticleEmitterBone = self:GetBone("CATRigHub004Bone001Bone002")
	self.mLeftLeg = self:GetBone("CATRigLLegAnkle2")
	self.mRightLeg = self:GetBone("CATRigRLegAnkle1")
	--local l_GameObject = self.m_LuaGameObject:GetGameObject()
	local l_CColliderName = self.m_LuaGameObject:GetName().."_CharacterCollider"
	
	
	
	l_ACName = self.m_LuaGameObject:GetName().."_AnimatorController"
	g_AnimatorControllerManager:AddComponent(l_ACName, self.m_LuaGameObject)
	

	local l_Idle = self.m_LuaGameObject:AddState("Idle_State", "idle", 1.0, "OnEnter_Idle_Dragon", "OnUpdate_Idle_Dragon", "OnExit_Idle_Dragon")
	local l_Rotate = self.m_LuaGameObject:AddState("Rotate_State", "idle", 1.0, "OnEnter_Rotate_Dragon", "OnUpdate_Rotate_Dragon", "OnExit_Rotate_Dragon")
	local l_ScratchRight = self.m_LuaGameObject:AddState("ScratchRight_State", "slash", 1.0, "OnEnter_ScratchRight_Dragon", "OnUpdate_ScratchRight_Dragon", "OnExit_ScratchRight_Dragon")
	local l_ScratchLeft = self.m_LuaGameObject:AddState("ScratchLeft_State", "slash2", 1.0, "OnEnter_ScratchLeft_Dragon", "OnUpdate_ScratchLeft_Dragon", "OnExit_ScratchLeft_Dragon")
	local l_SpitFire = self.m_LuaGameObject:AddState("SpitFire_State", "Spit", 1.0, "OnEnter_SpitFire_Dragon", "OnUpdate_SpitFire_Dragon", "OnExit_SpitFire_Dragon")
	--local l_Stomp = self.m_LuaGameObject:AddState("Stomp_State", "iraAttack", 1.0, "OnEnter_Stomp_Dragon", "OnUpdate_Stomp_Dragon", "OnExit_Stomp_Dragon")
	--local l_WhipTale = self.m_LuaGameObject:AddState("WhipTale_State", "iraAttack", 1.0, "OnEnter_WhipTale_Dragon", "OnUpdate_WhipTale_Dragon", "OnExit_WhipTale_Dragon")
    
	self.m_LuaGameObject:AddTrigger("IsPlayerInsideRightScratchRange", false)
	self.m_LuaGameObject:AddTrigger("IsPlayerInsideLeftScratchRange", false)
	self.m_LuaGameObject:AddTrigger("SpitFire", false)
	self.m_LuaGameObject:AddBool("Rotate", false)
	
	--self.m_LuaGameObject:AddTrigger("WaitingTimeExpired", false) --The time that the dragon waits beetwen attacks
	
	self.m_LuaGameObject:AddTrigger("IsHealthBellow25Percent", false)
	self.m_LuaGameObject:AddTrigger("IsHealthBellow50Percent", false)
	self.m_LuaGameObject:AddTrigger("IsHealthBellow75Percent", false)

	self.m_LuaGameObject:AddBool("HealthBellow25PercentAttackRaised", false)
	self.m_LuaGameObject:AddBool("HealthBellow50PercentAttackRaised", false)
	self.m_LuaGameObject:AddBool("HealthBellow75PercentAttackRaised", false)
	

	local l_IdleToSpitFire = l_Idle:AddTransition("IdleToSpitFire", l_SpitFire, false, 0.1)
	l_IdleToSpitFire:AddTriggerCondition("SpitFire")
	
	local l_IdleToScratchRight = l_Idle:AddTransition("IdleToScratchRight", l_ScratchRight, false, 0.2, 0.2)
	l_IdleToScratchRight:AddTriggerCondition("IsPlayerInsideRightScratchRange")
	
	local l_IdleToScratchLeft = l_Idle:AddTransition("IdleToScratchLeft", l_ScratchLeft, false, 0.2, 0.2)
	l_IdleToScratchLeft:AddTriggerCondition("IsPlayerInsideLeftScratchRange")
	
	local l_IdleToRotate = l_Idle:AddTransition("IdleToRotate", l_Rotate, false, 0.1)
	l_IdleToRotate:AddBoolCondition("Rotate",true)
	
	local l_RotateToIdle = l_Rotate:AddTransition("RotateToIdle", l_Idle, false, 0.1)
	l_RotateToIdle:AddBoolCondition("Rotate",false)
	
	local l_SpitFireToIdle = l_SpitFire:AddTransition("SpitFireToIdle", l_Idle, true, 0.1)
	local l_ScratchRightToIdle = l_ScratchRight:AddTransition("ScratchRightToIdle", l_Idle, true, 0.3)
	local l_ScratchLeftToIdle = l_ScratchLeft:AddTransition("ScratchLeftToIdle", l_Idle, true, 0.3)

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
	self:AddState(4.0, 10.0, 4.0, 15.0, 3.0, 2.0)
	self:AddState(3.0, 12.0, 2.0, 20.0, 3.0, 4.0)
	self:AddState(2.0, 15.0, 2.0, 30.0, 3.0, 4.0)
	self:AddState(1.0, 20.0, 2.0, 50.0, 3.0, 4.0)
	
	g_LogManager:Log("Dragon created...")
	
	
end

function CDragonComponent:Update(ElapsedTime)
	self:CheckLife()
	local l_Stuned = self:CheckStuned()
	self:SetParticleEmmiterPosition(self.m_ParticleEmitterBone)
end

function CDragonComponent:CreateSkeleton()
	
	if  #self.m_PhysicsHead > 0 then
		for i, value in ipairs(self.m_PhysicsHead) do
			if self.m_PhysicsHead[i].m_BoneType == "box" then
				self:CreateBoxTypeBone(self.m_PhysicsHead[i])
			elseif self.m_PhysicsHead[i].m_BoneType == "sphere" then
				self:CreateSphereTypeBone(self.m_PhysicsHead[i])
			end
		end	
	end	
	
	if  #self.m_PhysicsTail > 0 then
		for i, value in ipairs(self.m_PhysicsTail) do 
			if self.m_PhysicsTail[i].m_BoneType == "box" then
				self:CreateBoxTypeBone(self.m_PhysicsTail[i])
			elseif self.m_PhysicsTail[i].m_BoneType == "sphere" then
				self:CreateSphereTypeBone(self.m_PhysicsTail[i])
			end
		end
	end
	
	if  #self.m_PhysicsBody > 0 then
		for i, value in ipairs(self.m_PhysicsBody) do
			if self.m_PhysicsBody[i].m_BoneType == "box" then
				self:CreateBoxTypeBone(self.m_PhysicsBody[i])
			elseif self.m_PhysicsBody[i].m_BoneType == "sphere" then
				self:CreateSphereTypeBone(self.m_PhysicsBody[i])
			end
		end
	end	

end

function CDragonComponent:CreateBoxTypeBone(Bone)
	local l_bone = Bone
	local l_BoneSize = l_bone.m_BoneSize * 2
	local l_BoneWorldPos = l_bone:GetWorldPos()
	--local l_BoneRotation = l_bone:GetRotation()
	local l_BoneRotation = Quatf(-0.688528, -0.161031, 0.146242, -0.691818)	
	
	--local l_result = g_PhysXManager:CreateBoxTrigger(l_bone.m_BoneName, l_bone.m_BoneName, l_BoneSize ,l_bone.m_BoneName.."mat", "GROUP3",l_BoneWorldPos,l_BoneRotation, "kinematic")
	--CreateBoxLua(const std::string &ShapeName, const Vect3f &Size, const std::string MaterialName, float MaterialStaticFriction, float MaterialDynamicFriction, float MaterialRestitution, const std::string &Group, bool IsExclusive)
	g_PhysXManager:CreateBoxLua(l_bone.m_BoneName, l_BoneSize, l_bone.m_BoneName.."mat", 10.0, 20.0, 1.0, "", true)
	local l_result = g_PhysXManager:CreateDynamicActor(l_bone.m_BoneName, l_bone.m_BoneName, l_BoneWorldPos, l_BoneRotation, 5.0, true);
	if l_result then
		-- g_LogManager:Log(l_bone.m_BoneName.." BoxSkeleton Creado OK ") 
	else
		 g_LogManager:Log(l_bone.m_BoneName.." BoxSkeleton Creado Mal ")
	end
end

function CDragonComponent:CreateSphereTypeBone(Bone)
	
	local l_bone = Bone
	local l_BoneSize = l_bone.m_BoneSize
	local l_BoneWorldPos = l_bone:GetWorldPos()
	local l_BoneRotation = l_bone:GetRotation()		
	--local l_result = g_PhysXManager:CreateSphereTrigger(l_bone.m_BoneName, l_bone.m_BoneName, l_BoneSize ,l_bone.m_BoneName.."mat", "GROUP3",l_BoneWorldPos,l_BoneRotation, "kinematic")
	g_PhysXManager:CreateSphereLua(l_bone.m_BoneName, l_BoneSize, l_bone.m_BoneName.."mat", 10.0, 20.0, 1.0, "", true)
	local l_result = g_PhysXManager:CreateDynamicActor(l_bone.m_BoneName, l_bone.m_BoneName, l_BoneWorldPos, l_BoneRotation, 5.0, true);
	if l_result then
		 --g_LogManager:Log(l_bone.m_BoneName.." SphereSkeleton Creado OK ")
	else
		 g_LogManager:Log(l_bone.m_BoneName.." SphereSkeleton Creado Mal ")
	end
end	

function CDragonComponent:AddPxPiece(Index, Piece)
	
	if Index == "head" then
		--g_LogManager:Log("Agregando Dragon Piece: "..Piece.m_BoneName.." Tipo: "..Index)
		table.insert(self.m_PhysicsHead, Piece)
	elseif Index == "body" then
		--g_LogManager:Log("Agregando Dragon Piece: "..Piece.m_BoneName.." Tipo: "..Index)
		table.insert(self.m_PhysicsBody, Piece)
	elseif Index == "tail" then
		--g_LogManager:Log("Agregando Dragon Piece: "..Piece.m_BoneName.." Tipo: "..Index)
		table.insert(self.m_PhysicsTail, Piece)
	else
		g_LogManager:Log("Error al crear un hueso del dragon, indice no reconocido")
	end 
	--g_LogManager:Log("Dragon Piece: "..Piece.m_BoneName.." Agregada")
end

function CDragonComponent:GetBone(BoneName)
	
	if  #self.m_PhysicsTail > 0 then
		for i, value in ipairs(self.m_PhysicsTail) do
			if self.m_PhysicsTail[i].m_BoneName == BoneName then
				
				return self.m_PhysicsTail[i]
			end
		end
	
	end
	if #self.m_PhysicsBody > 0 then
		
		for j, value in ipairs(self.m_PhysicsBody) do
			if self.m_PhysicsBody[j].m_BoneName == BoneName then
				return self.m_PhysicsBody[j]
			end
		end
	end	
	
	if #self.m_PhysicsHead > 0 then
		for k, value in ipairs(self.m_PhysicsHead) do		
			if self.m_PhysicsHead[k].m_BoneName == BoneName then
				return self.m_PhysicsHead[k]
			end
		end
	end	
	
	g_LogManager:Log("Metodo CDragonComponent:GetBone(BoneName) invocado pero no consiguio el hueso")
	
	
end

function CDragonComponent:ChangeTailState(State)
	if  #self.m_PhysicsTail > 0 then
		for i, value in ipairs(self.m_PhysicsTail) do
			self.m_PhysicsTail[i]:ChangeTriggerState(State)
		end
	end
end

function CDragonComponent:ChangeAttackingLegsState(State)
	self.mLeftLeg:ChangeTriggerState(State)
	self.mRightLeg:ChangeTriggerState(State)
end

function CDragonComponent:GetLuaGameObject() return self.m_LuaGameObject end
--function CDragonComponent:GetSpeed()	return self.m_Speed end
function CDragonComponent:GetSpeed()return self.m_Speed end
function CDragonComponent:GetArmor() return self.m_Armor end
function CDragonComponent:GetHealth() return self.m_Health end
function CDragonComponent:StopSpittingFire() self.m_FireParticles:EnableRenderableObject(false) end
function CDragonComponent:SpitFire() self.m_FireParticles:EnableRenderableObject(true) end
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
			l_Result = (CurrentYaw + 1.5708)+ (Angle*ElapsedTime)
		else l_Result = (CurrentYaw + 1.5708) + (Velocity*ElapsedTime)
		end
	end
	return l_Result
end

function CDragonComponent:LookAtPoint(Point, ElapsedTime)
	local l_Forward = self.m_LuaGameObject:GetForward()
	l_Forward.y = 0.0
	--Point.y = 0.0
	local l_Angle = CTTODMathUtils.GetAngleToFacePoint(l_Forward, self.m_LuaGameObject:GetPosition(), Point)	
	-- -k*T    si el ángulo es negativo. K es la velocidad
	--  si -k*T < angulo, aplicas el angulo, y no la formula, para que no se pase de rotación
	local l_CurrentYaw = self.m_LuaGameObject:GetYaw()
	local l_Velocity = self.m_CurrentState.m_RotationVelocity
	
	self.m_LuaGameObject:SetYaw(self:CalculateNewAngle(l_Angle, l_CurrentYaw, l_Velocity, ElapsedTime))
end

function CDragonComponent:CheckStuned(ElapsedTime)
	if self.m_TimeStuned>0 then
		self.m_TimeStuned = self.m_TimeStuned - ElapsedTime
		return true
	end
	return false
end

function CDragonComponent:CheckLife()
	if self.m_Health<=0 then
		g_EventManager:FireEvent("DragonIsDead")
	end
end

function CDragonComponent:GetHealthPercentage()
	return ((self.m_Health*100)/self.m_MaxHealth) 
end


function CDragonComponent:TakeDamage(PlayerWeapon, PlayerDamage)
	local l_DragonArmor = self.m_Armor:GetType()
	self.m_Health = self.m_Health - (g_DamageCalculator:CalculateDamage(l_DragonArmor,PlayerWeapon,PlayerDamage))
end
function CDragonComponent:TakeMagicDamage(Damage)
	self.m_Health = self.m_Health - Damage
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

function CDragonComponent:ChangeAttackState(value)
	
	if value < #self.m_States then
		self.m_CurrentStateIndex = value
		self.m_CurrentState = self.m_States[value]
		g_LogManager:Log("DragonAttackState: State changed to "..value)
	else
		g_LogManager:Log("DragonAttackState: Invalid State")
	end
	--g_LogManager:Log("Dragon changes to state "..self.m_CurrentStateIndex)
end
function CDragonComponent:GetCurrentState()
	return self.m_CurrentState
end

function CDragonComponent:SetParticleEmmiterPosition(Bone)
	local l_Pos = Bone:GetWorldPos()
	self.m_FireParticles:SetPosition(l_Pos)
end


function CDragonComponent:SetAttackFacingValues(ForwardBeforeFacing,DirectionToFace)	
	self:SetFacingValues(ForwardBeforeFacing,DirectionToFace,"Rotation_State", 2.0)
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

------------------------------------------

function CDragonComponent:SetFacingValues(ForwardBeforeFacing,DirectionToFace, StateName, Velocity)	
	local l_Pi = 3.14159265359
	self.m_RotationAngle = CTTODMathUtils.AngleBetweenVectors(DirectionToFace,ForwardBeforeFacing) 

	if(self.m_RotationAngle>l_Pi)then
		self.m_RotationAngle = self.m_RotationAngle - (l_Pi*2)
	elseif(self.m_RotationAngle<(-l_Pi))then
		self.m_RotationAngle = self.m_RotationAngle + (l_Pi*2)
	end
	
	self.m_RotationDuration = (2.0)/Velocity
	
	g_LogManager:Log("RotationAngle: "..self.m_RotationAngle)
	self.m_YawBeforeFacing = self.m_LuaGameObject:GetYaw()
	
	g_LogManager:Log("YawBeforeFacing: "..self.m_YawBeforeFacing)
	
	
	
	local l_NewYaw = CTTODMathUtils.GetFixedAngle(self.m_LuaGameObject:GetYaw() + self.m_RotationAngle)
	self.m_YawAfterFacing = l_NewYaw
	g_LogManager:Log("YawAfterFacing: "..self.m_YawAfterFacing)
	
	-- g_LogManager:Log("Settings values")
	-- g_LogManager:Log("m_YawBeforeFacing"..self.m_YawBeforeFacing)
	-- g_LogManager:Log("m_YawAfterFacing"..self.m_YawAfterFacing)
end

function CDragonComponent:FaceDirection(ElapsedTime)
	g_LogManager:Log("Entra en el FaceDirection")
	if(self.m_RotationDuration == 0.0) then
		g_LogManager:Log("Error. RotationDuration is 0")
	end
		
	local l_Pi = 3.14159265359
	local l_Pi_2 = (l_Pi/2.0)
	
	local ThreeQuarter = l_Pi + l_Pi_2
	local l_CurrentYaw = self.m_LuaGameObject:GetYaw()
	local l_Difference = self.m_YawAfterFacing - self.m_LuaGameObject:GetYaw()
	
	if((l_CurrentYaw>ThreeQuarter)and(self.m_YawAfterFacing<(l_Pi/2.0))) then
		l_Difference = (2*l_Pi) - l_CurrentYaw + self.m_YawAfterFacing
	elseif((l_CurrentYaw<0.0) and(self.m_YawAfterFacing<0.0)) then
		l_Difference = math.abs(math.abs(l_CurrentYaw) - math.abs(self.m_YawAfterFacing))
	end
	
	l_Difference = math.abs(l_Difference)
	
	if (l_Difference>self.m_AngleMargin) then
		g_LogManager:Log("self.m_RotationDuration: "..self.m_RotationDuration)
		g_LogManager:Log("self.m_RotationAngle: "..self.m_RotationAngle)
		--g_LogManager:Log("GetTimer: "..self:GetTimer())
		local l_Angle = (self:GetTimer()*self.m_RotationAngle)/self.m_RotationDuration
		local l_NewYaw = CTTODMathUtils.GetFixedAngle(self.m_YawBeforeFacing + l_Angle)
		g_LogManager:Log("l_Angle: "..l_Angle)
		g_LogManager:Log("l_NewYaw: "..l_NewYaw)
		if(l_NewYaw<0.0 and self.m_YawAfterFacing<0.0) then
			if(self.m_RotationAngle<0.0 and l_NewYaw<self.m_YawAfterFacing)then
				g_LogManager:Log("m_FacingFinished1")
				self.m_FacingFinished = true
				self.m_LuaGameObject:SetYaw(self.m_YawAfterFacing)
				return true
			elseif(self.m_RotationAngle>0.0 and l_NewYaw>self.m_YawAfterFacing)then
				self.m_LuaGameObject:SetYaw(self.m_YawAfterFacing)
				g_LogManager:Log("m_FacingFinished2")
				self.m_FacingFinished = true
				return true
			end
		elseif(l_NewYaw>0.0 and self.m_YawAfterFacing>0.0) then
			if(self.m_RotationAngle<0.0 and l_NewYaw<self.m_YawAfterFacing)then
				g_LogManager:Log("m_FacingFinished3")
				self.m_FacingFinished = true
				self.m_LuaGameObject:SetYaw(self.m_YawAfterFacing)
				return true
			elseif(self.m_RotationAngle>0.0 and l_NewYaw>self.m_YawAfterFacing)then
				self.m_LuaGameObject:SetYaw(self.m_YawAfterFacing)
				g_LogManager:Log("m_FacingFinished4")
				self.m_FacingFinished = true
				return true
			end
		end
		
		self.m_LuaGameObject:SetYaw(l_NewYaw)
	else 
		self.m_LuaGameObject:SetYaw(self.m_YawAfterFacing)
		g_LogManager:Log("m_FacingFinished5")
		self.m_FacingFinished = true
	end
end	

