-- function CreatePlayer(AnimatedInstanceModel)
	-- dofile("./Data/Scripting/Player/Player.lua")
	-- --g_Player = CPlayerComponent(AnimatedInstanceModel)
	
	-- local l_Player=CPlayerComponent(AnimatedInstanceModel)
	-- AnimatedInstanceModel:AddComponent(l_Player)
	
	-- g_LogManager:Log("Player created...")
-- end

dofile("./Data/Scripting/Player/ItemGui.lua")
dofile("./Data/Scripting/Player/Item.lua")
dofile("./Data/Scripting/Player/HealthPotion.lua")
dofile("./Data/Scripting/Player/Slot.lua")
dofile("./Data/Scripting/Player/Inventory.lua")

class 'CPlayerComponent' (CLUAComponent)
function CPlayerComponent:__init(CLuaGameObject)
	CLUAComponent.__init(self,CLuaGameObject:GetName().."_PlayerScript")
	self.m_LuaGameObject = CLuaGameObject 
	self.m_MaxHealth=700
	self.m_Health=self.m_MaxHealth
	self.m_Dead = false
	self.m_LowHealth = false
	self.m_FullHealth = false
	self.m_Healing = false
	self.m_HealPoints = 0.0
	self.m_HealVelocity = 12.0
	
	self.m_BurningState = false
	self.m_BurningStateTime = 2.0
	self.m_BurningStateCounter = 0.0
	
	self.m_CountdownToExtintionTimer = 5.0
	self.m_CountdownToExtintion = self.m_CountdownToExtintionTimer
	
	self.m_Speed=1.0
	self.m_AttackDelay = 1.0
	self.m_MinBlockAngle = 1.74
	self.m_MaxBlockAngle = 4.53
	
	self.m_AttackingFacingDistance = 2.0
	self.m_BeeingTossed = false 
	self.m_TossedDirection = Vect3f(0.0,0.0,0.0)
	
	self.m_Height = 1.4
	self.m_Density = 30.0
	self.m_Radius = 0.3
	
	self.m_Armors={}
	self.m_Weapons={}
	self.m_CurrentArmor = nil
	self.m_CurrentWeapon = nil
	self.m_Inventory = CInventory()
	
	self.m_Locked = false
	
	--Animator 
	self.m_Walk = false
	self.m_Rotation = false
	self.m_Forward = false
	self.m_Backwards = false
	self.m_Right = false
	self.m_Left = false
	
	self.m_RotationVelocity = 8.0
	self.m_AngleMargin = 0.15
	
	--Facing direcition variables
	self.m_RotationAngle = 0.0
	self.m_RotationDuration = 0.0
	self.m_YawBeforeFacing = 0.0
	self.m_YawAfterFacing = 0.0
	
	self.m_FacingFinished = false
	self.m_AttackDisplacement = 0.5
	self.m_DistanceToEnterCombatIdle = 5.0
	
	self.m_Velocity = Vect3f(0.0,0.0,0.0)
	self.m_Speed = 3.0
	
	--Actions
	self.m_MapOpened = false
	self.m_Attacking = false
	self.m_Blocking = false
	self.m_Interacting = false
	
	self.m_AttackFinished = true
	self.m_BeeingTossed = false
	
	self:SubscribeEvents()
end

-- Events
function CPlayerComponent:SubscribeEvents()
	g_EventManager:Subscribe(self, "PLAYER_WALKING_FORWARD")
	g_EventManager:Subscribe(self, "PLAYER_WALKING_BACKWARDS")
	g_EventManager:Subscribe(self, "PLAYER_WALKING_RIGHT")
	g_EventManager:Subscribe(self, "PLAYER_WALKING_LEFT")

	g_EventManager:Subscribe(self, "PLAYER_LOW_HEALTH")
	g_EventManager:Subscribe(self, "PLAYER_FULL_HEALTH")
	g_EventManager:Subscribe(self, "LOCK_CHARACTER")
	g_EventManager:Subscribe(self, "PLAYER_OPENS_MAP")
	g_EventManager:Subscribe(self, "PLAYER_DRINKS_BEER")
	g_EventManager:Subscribe(self, "PLAYER_ATTACKS")
	g_EventManager:Subscribe(self, "PLAYER_BLOCKS")
	g_EventManager:Subscribe(self, "ON_ALT8_PRESSED")
	g_EventManager:Subscribe(self, "DYNAMITE_DROPPED")
end

function CPlayerComponent:ON_ALT8_PRESSED()
end

dofile("./Data/Scripting/Player/PlayerEventsHandler.lua")

function CPlayerComponent:AddVelocity(Vector)
	self.m_Velocity = self.m_Velocity + (Vector * self.m_Speed)
end

function CPlayerComponent:AddGravity()
	self.m_Velocity = self.m_Velocity + Vect3f(0.0,-10.0,0.0)
end

-- Facing

function CPlayerComponent:SetAttackFacingValues(ForwardBeforeFacing,DirectionToFace)	
	self:SetFacingValues(ForwardBeforeFacing,DirectionToFace,"Attack_State", 2.0)
end

function CPlayerComponent:SetInteractionFacingValues(ForwardBeforeFacing,DirectionToFace)	
	self:SetFacingValues(ForwardBeforeFacing,DirectionToFace,"Interact_State", 4.0)
end

function CPlayerComponent:SetFacingValues(ForwardBeforeFacing,DirectionToFace, StateName, Velocity)	
	local l_Pi = 3.14159265359
	self.m_RotationAngle = CTTODMathUtils.AngleBetweenVectors(DirectionToFace,ForwardBeforeFacing) 

	if(self.m_RotationAngle>l_Pi)then
		self.m_RotationAngle = self.m_RotationAngle - (l_Pi*2)
	elseif(self.m_RotationAngle<(-l_Pi))then
		self.m_RotationAngle = self.m_RotationAngle + (l_Pi*2)
	end
	
	self.m_RotationDuration = (self.m_LuaGameObject:GetState(StateName):GetCurrentAnimation().m_Duration)/Velocity
	self.m_YawBeforeFacing = self.m_LuaGameObject:GetYaw()
	
	local l_NewYaw = CTTODMathUtils.GetFixedAngle(self.m_LuaGameObject:GetYaw() + self.m_RotationAngle)
	self.m_YawAfterFacing = l_NewYaw
	
	-- g_LogManager:Log("Settings values")
	-- g_LogManager:Log("m_YawBeforeFacing"..self.m_YawBeforeFacing)
	-- g_LogManager:Log("m_YawAfterFacing"..self.m_YawAfterFacing)
end

dofile("./Data/Scripting/Player/PlayerInitialize.lua")

dofile("./Data/Scripting/Player/PlayerController.lua")

function CPlayerComponent:Update(ElapsedTime)
	if(not self.m_Dead) then
		self.m_Inventory:Update(ElapsedTime)
	end
	
	if (self.m_BurningState) then
		self:SetBurningStateTimer(ElapsedTime)
	end
	
	if self.m_Healing then
		self:Healing(ElapsedTime)
	end
	
	self:PlayerController(ElapsedTime)
	self:HandleEvents()
	
	if (self.m_Dead) then
		self:CountdownToExtintion(ElapsedTime)
	end
end

function CPlayerComponent:Die()
	self.m_Dead = true
	self.m_LuaGameObject:PlayEvent("PlayerNotLowHealth")
	self:Lock()
	g_EventManager:FireEvent("CENTER_CAMERA")
	g_Engine:SetTimeScale(0.65)
	-- Change background music
end

function CPlayerComponent:CountdownToExtintion(ElapsedTime)
	if (self.m_CountdownToExtintionTimer>=0.0) then
		self.m_CountdownToExtintionTimer = self.m_CountdownToExtintionTimer - ElapsedTime
	end
	-- local l_Material = g_MaterialManager:GetResource("player_dead_fadeout_material")
	-- l_Material:SetFloatParameterValue("alpha",l_Alpha)
	
	if (self.m_CountdownToExtintionTimer<=0.0) then
		g_EventManager:FireEvent("PLAYER_IS_DEAD")
		--self.m_LuaGameObject:PlayEvent("PlayerDeadSound")
		--self.m_LuaGameObject:PlayEvent("PlayerDeadMusic")
	end
end

function CPlayerComponent:PLAYER_LOW_HEALTH()
	self.m_LowHealth = true
	self.m_LuaGameObject:PlayEvent("PlayerLowHealth")
end
function CPlayerComponent:PLAYER_FULL_HEALTH()
	self.m_FullHealth = true
	--self.m_LuaGameObject:PlayEvent("PlayerFullHealth")
end

function CPlayerComponent:HandleEvents()
	if (self:FullHealth() and (not self.m_FullHealth)) then
		g_EventManager:FireEvent("PLAYER_FULL_HEALTH")
	elseif (self:LowHealth() and (not self.m_LowHealth)) then
		g_EventManager:FireEvent("PLAYER_LOW_HEALTH")
	end
end

function CPlayerComponent:Lock() self.m_Locked = true end
function CPlayerComponent:Unlock() self.m_Locked = false end
function CPlayerComponent:IsLocked() return self.m_Locked end

function CPlayerComponent:IsBeingTossed(Value) self.m_BeeingTossed = Value end

function CPlayerComponent:FullHealth()
	return (self.m_Health==self.m_MaxHealth)
end
function CPlayerComponent:LowHealth()
	return (self.m_Health<(self.m_MaxHealth*0.25))
end
function CPlayerComponent:GetLuaGameObject() return self.m_LuaGameObject end
function CPlayerComponent:GetHealth() return self.m_Health end
function CPlayerComponent:GetMaxHealth() return self.m_MaxHealth end
function CPlayerComponent:GetSpeed() return self.m_Speed end
function CPlayerComponent:GetMinBlockAngle() return self.m_MinBlockAngle end
function CPlayerComponent:GetMaxBlockAngle() return self.m_MaxBlockAngle end
function CPlayerComponent:SetTossedDirection(Direction) self.mTossedDirection = Direction end

function CPlayerComponent:GetReference() return self end
function CPlayerComponent:IsMapOpened() return self.m_MapOpened end

function CPlayerComponent:AddArmor(CArmorComponent)
	table.insert(self.m_Armors, CArmorComponent)
	if self.m_CurrentArmor == nil then
		self.m_CurrentArmor = CArmorComponent
	end
end
function CPlayerComponent:AddWeapon(CWeaponComponent)
	table.insert(self.m_Weapons, CWeaponComponent)	
	if self.m_CurrentWeapon == nil then
		self.m_CurrentWeapon = CWeaponComponent
	end
end
function CPlayerComponent:ChangeArmor(Index) self.m_CurrentArmor = m_Armors[Index] end
function CPlayerComponent:ChangeWeapon(Index) self.m_CurrentWeapon = m_Weapons[Index] end
function CPlayerComponent:GetArmor(Index) return m_Armors[Index] end
function CPlayerComponent:GetWeapon(Index) return m_Weapons[Index] end

dofile("./Data/Scripting/Player/PlayerCombatMechanics.lua")

function CPlayerComponent:OpenMap()
	self.m_MapOpened = not self.m_MapOpened
	if (self.m_MapOpened) then
		self:Lock()
	else
		self:Unlock()
	end
	--self.m_AudioSource:PlayEvent("OpenMapSound")
end

function CPlayerComponent:DrinkBeer()
	self.m_Inventory:UseItem(1)
end

function CPlayerComponent:Healing(ElapsedTime) 
	local l_HPoints = ElapsedTime*self.m_HealVelocity
	if((self.m_Health + l_HPoints)>self.m_MaxHealth) then
		self.m_Health = self.m_MaxHealth
		self.m_Healing = false
		self.m_LuaGameObject:PlayEvent("PlayerNotLowHealth")
		self.m_LowHealth = false
	else
		self.m_Health = self.m_Health + l_HPoints 
	end
	
	self.m_HealPoints = self.m_HealPoints - l_HPoints
	if(self.m_HealPoints<=0.0) then
		self.m_Healing = false
		self.m_LuaGameObject:PlayEvent("PlayerNotLowHealth")
		self.m_LowHealth = false
	end
end

function CPlayerComponent:Heal(HealPoints) 
	self.m_Healing = true
	self.m_HealPoints = HealPoints
end
