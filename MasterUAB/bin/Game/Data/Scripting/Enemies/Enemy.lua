class 'CEnemyComponent' (CLUAComponent)
function CEnemyComponent:__init(CLuaGameObject,EnemyType)
	CLUAComponent.__init(self,CLuaGameObject:GetName().."_"..EnemyType.."Script")
	self.m_LuaGameObject = CLuaGameObject
	self.m_Health = 100.0
	self.m_MaxHealth = 100.0
	self.m_Dead = false
	self.m_CountdownToExtintionTimer = 6.0
	self.m_CountdownToRemoveWeapon = self.m_CountdownToExtintionTimer*0.95
	self.m_CountdownToExtintion = self.m_CountdownToExtintionTimer
	
	self.m_Armor = nil
	self.m_Weapon = nil
	self.m_WeaponsRemoved = false
	
	self.m_Speed = 0.0
	self.m_Velocity = Vect3f(0.0,0.0,0.0)
	self.m_AttackDelay = 0.0
	self.m_VisionRange = 0.0
	self.m_RotationVelocity = 3.5
	
	self.m_DelayToPatrol = 2.0
	self.m_WayPoints = {}
	self.m_CurrentWayPoint = 1
	
	--Character collider
	self.m_Height = 1.4
	self.m_Density = 30
	self.m_Radius = 0.3
	
	--Movements
	self.m_FollowWaypoints = false
	self.m_AttackMovement = false
	
	--Conditionals
	self.m_Attacking = false
	-- self.m_GotHit = false
	
	self.m_GuiAvatar = nil
	self.m_DieDelay = 5.0
	
	self.m_DistanceToFacePlayer = 5.0
	
	self.m_DeadEvent = ""
	self.m_DeadEventParameter = nil
	
	self.m_TemporalMaterials = nil
end

function CEnemyComponent:Initialize()
	local l_ComponentName = self.m_LuaGameObject:GetName().."_CharacterCollider"
	g_PhysXManager:AddCharacterColliderComponent(self.m_LuaGameObject:GetName(), self.m_LuaGameObject, self.m_Height, self.m_Radius, self.m_Density)
	
	g_AnimatorControllerManager:AddComponent(self.m_LuaGameObject:GetName().."_AnimatorController", self.m_LuaGameObject)
	
	g_SoundManager:AddComponent(self.m_LuaGameObject:GetName().."_AudioSource", self.m_LuaGameObject)
	
	g_LogManager:Log("Enemy "..self.m_LuaGameObject:GetName().." created...")
end

function CEnemyComponent:Update(ElapsedTime)
	if (self.m_Dead) then
		self:CountdownToExtintion(ElapsedTime)
	end
end

function CEnemyComponent:AddWaypoint(WayPoint)
	table.insert(self.m_WayPoints,WayPoint)
end

function CEnemyComponent:TakeDamage(PlayerWeapon)
	if (self:IsAttacking()==false)then self:GetLuaGameObject():SetTrigger("GotHit") end
	g_EventManager:FireEvent("ENEMY_TAKES_DAMAGE") 
	if g_ShowHealthBarManager ~= nil then
		g_ShowHealthBarManager:SetLastEnemyHit(self)
	end
	local l_Armor = ""
	if self.m_Armor ~= nil then l_Armor = self.m_Armor:GetType() end
	local l_PlayerDamage = PlayerWeapon:GetDamage()
	local l_DamageCalculated = g_DamageCalculator:CalculateDamage(l_Armor,PlayerWeapon,l_PlayerDamage)
	if self.m_Health > 0.0 then
		if((self.m_Health - l_DamageCalculated)<=0.0) then
			self.m_Health = 0.0
			self:Die()
		else
			self.m_Health = self.m_Health - l_DamageCalculated
		end
	end
end

function CEnemyComponent:Die()
	--self.m_LuaGameObject:PlayEvent("EnemyDie")
	--local l_RenderableObjectTechnique = g_RenderableObjectTechniqueManager:GetResource("fade_out_renderable_object_technique")
	--self.m_LuaGameObject:SetTemporalRenderableObjectTechnique(l_RenderableObjectTechnique)
	self.m_Dead = true
	g_LayerManager:ChangeElementFromLayer(self.m_LuaGameObject:GetName(),"solid","dead_enemies")
	self.m_LuaGameObject:SetBool("Dead",true)
	self.m_TemporalMaterials = self.m_LuaGameObject:CreateCopyMaterialsFromCore()
	for i=0,(self.m_TemporalMaterials:size()-1) do
		self.m_TemporalMaterials:at(i):AddTexture(g_FadeOutNoiseTexture,false)
	end
end

function CEnemyComponent:CountdownToExtintion(ElapsedTime)
	self.m_CountdownToExtintionTimer = self.m_CountdownToExtintionTimer - ElapsedTime
	local l_CutOut = (1.0 - (self.m_CountdownToExtintionTimer/self.m_CountdownToExtintion))  
	if l_CutOut > 1.0 then
		l_CutOut = 1.0
	end
	
	for i=0,(self.m_TemporalMaterials:size()-1) do
		self.m_TemporalMaterials:at(i):SetFloatParameterValue("cutout_factor",l_CutOut)
	end
	
	if(self.m_CountdownToExtintionTimer <= self.m_CountdownToRemoveWeapon and self.m_WeaponsRemoved==false) then
		local l_Name = self.m_LuaGameObject:GetName()
		g_GameController:RemoveEnemyArmorAndWeapon(l_Name)
		self.m_WeaponsRemoved = true
		g_LuaGameObjectHandleManager:RemoveCharacterCollider(l_Name)
	end
	
	if(self.m_CountdownToExtintionTimer <= 0.0) then
		self:RaiseDeadEvents()
		--self.m_LuaGameObject:SetTemporalRenderableObjectTechnique(nil)
		g_GameController:RemoveElementFromTable(g_GameController:GetEnemies(),self.m_LuaGameObject:GetName())
	end
end

function CEnemyComponent:RaiseDeadEvents()
	if (self.m_DeadEvent~="") then
		if self.m_DeadEvent == "DYNAMITE_DROPPED" then
			g_EventManager:FireEvent(self.m_DeadEvent, self.m_LuaGameObject:GetPosition())
		else 
			g_EventManager:FireEvent(self.m_DeadEvent)
		end
	end
end

function CEnemyComponent:CalculateNewAngle(Angle, CurrentYaw, Velocity, ElapsedTime)
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

function CEnemyComponent:LookAtPoint(Point, ElapsedTime)
	local l_Forward = self.m_LuaGameObject:GetForward()
	l_Forward.y = 0.0
	local l_Angle = CTTODMathUtils.GetAngleToFacePoint(l_Forward, self.m_LuaGameObject:GetPosition(), Point)	
	-- -k*T    si el ángulo es negativo. K es la velocidad
	--  si -k*T < angulo, aplicas el angulo, y no la formula, para que no se pase de rotación
	local l_CurrentYaw = self.m_LuaGameObject:GetYaw()
	-- if(l_Angle > 0.05 or l_Angle < -0.05) then
		self.m_LuaGameObject:SetYaw(self:CalculateNewAngle(l_Angle, l_CurrentYaw, self.m_RotationVelocity, ElapsedTime))
	-- end
end

function CEnemyComponent:FollowTriangleWayPoints(ElapsedTime)
	local l_Position = self.m_LuaGameObject:GetPosition()
	if (#self.m_WayPoints)>0 then
		local l_Destiny = Vect3f(0.0,0.0,0.0)
		l_Destiny = self.m_WayPoints[self.m_CurrentWayPoint]
		local l_Vector =  l_Destiny - l_Position
		
		if CTTODMathUtils.PointInsideCircle(l_Destiny, l_Position, 0.2) == false then
			l_Vector:Normalize(1)
			self.m_Velocity = (l_Vector*self:GetSpeed())
			self:LookAtPoint(l_Destiny, ElapsedTime)
		else
			self.m_CurrentWayPoint = self.m_CurrentWayPoint+1
		end
		
		if self.m_CurrentWayPoint > (#self.m_WayPoints) then
			self.m_CurrentWayPoint = 1
		end
	end
end

function CEnemyComponent:MoveWithAStar(ElapsedTime)
	
	local l_Enemy = self.m_LuaGameObject
	local l_EnemyPos = l_Enemy:GetPosition()
	
	local l_PlayerPos = g_Player:GetPosition()
	local l_Pos = Vect3f(0.0,0.0,0.0)
	local l_Points = g_AIManager:SearchAStarPath(l_EnemyPos,l_PlayerPos,false)
	
	if (l_Points~= nil and l_Points:size() >= 0) then
	
		local l_VectorToPlayer = Vect3f(0.0,0.0,0.0)
		local l_TotalPoints = l_Points:size()
			
		for i=0,(l_TotalPoints-1) do
			l_Pos = l_Points:at(i)
					
			if CTTODMathUtils.PointInsideCircle(l_EnemyPos, l_Pos, 0.9) then
				local l_next = i +1
					l_VectorToPlayer = l_Points:at(l_next) - l_EnemyPos
				break
			end
		end
				
		l_VectorToPlayer:Normalize(1.0)
		self.m_Velocity.x = 0.0
		self.m_Velocity.z = 0.0
		self.m_Velocity = self.m_Velocity + (l_VectorToPlayer*self:GetSpeed())
		
		local l_Forward = self.m_LuaGameObject:GetForward()
		--local NameOfHit = g_PhysXManager:RaycastOutName(l_EnemyPos, l_Forward, self.m_DistanceToFacePlayer)
		
		local l_RotateView = false
		local l_Distance = (l_PlayerPos-self.m_LuaGameObject:GetPosition()):Length()
		if l_Distance < self.m_DistanceToFacePlayer then 
			l_RotateView = true
		end
		
		if ElapsedTime > 0.0 then
			if l_RotateView then
				self:LookAtPoint(l_PlayerPos,ElapsedTime)
			end	
		else 
			if l_RotateView then
				self:LookAtPoint(l_PlayerPos,0.0)
			end
		end 
	end
end

function CEnemyComponent:SetDeadEvent(Event, Parameter) self.m_DeadEvent = Event self.m_DeadEventParameter=Parameter end
function CEnemyComponent:GetAvatar() return self.m_GuiAvatar end
function CEnemyComponent:GetHealth() return self.m_Health end
function CEnemyComponent:GetMaxHealth() return self.m_MaxHealth end
function CEnemyComponent:GetSpeed()	return self.m_Speed end
function CEnemyComponent:GetVisionRange() return self.m_VisionRange end
function CEnemyComponent:GetAttackDelay() return self.m_AttackDelay end
function CEnemyComponent:GetArmor()	return self.m_Armor end
function CEnemyComponent:SetArmor(Armor) self.m_Armor = Armor end
function CEnemyComponent:GetWeapon() return self.m_Weapon end
function CEnemyComponent:SetWeapon(Weapon) self.m_Weapon = Weapon end
function CEnemyComponent:GetLuaGameObject() return self.m_LuaGameObject end
function CEnemyComponent:IsAttacking() return self.m_Attacking end
function CEnemyComponent:SetAttacking(state) self.m_Attacking = state end
-- function CEnemyComponent:IsHit() return self.m_GotHit end
-- function CEnemyComponent:SetHitState(state) self.m_GotHit = state end
function CEnemyComponent:SetFollowTriangleWayPointsState(Value) self.m_FollowWaypoints = Value end
function CEnemyComponent:SetMoveToAttackMovement(Value) self.m_AttackMovement = Value end