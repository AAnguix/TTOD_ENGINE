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
	self.m_MaxHealth=20.0
	self.m_Health=20.0
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
	
	self.m_RotationVelocity = 4.0
	self.m_AngleMargin = 0.05
	self.m_AttackDirection = Vect3f(0.0,0.0,0.0)
	self.m_AttackDisplacement = 0.01
	
	self.m_Velocity = Vect3f(0.0,0.0,0.0)
	self.m_Speed = 3.0
	
	--Other
	self.m_MapOpened = false
	self.m_IsBlocking = false
	self.m_Attacking = false
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

	g_EventManager:Subscribe(self, "LOCK_CHARACTER")
	g_EventManager:Subscribe(self, "PLAYER_OPENS_MAP")
	g_EventManager:Subscribe(self, "PLAYER_DRINKS_BEER")
	g_EventManager:Subscribe(self, "PLAYER_ATTACKS")
	g_EventManager:Subscribe(self, "PLAYER_BLOCKS")
	g_EventManager:Subscribe(self, "ON_ALT8_PRESSED")
end

function CPlayerComponent:ON_ALT8_PRESSED()
end

dofile("./Data/Scripting/Player/PlayerEventsHandler.lua")

function CPlayerComponent:AddVelocity(Vector)
	self.m_Velocity = self.m_Velocity + (Vector * self.m_Speed)
end

function CPlayerComponent:AddGravity(ElapsedTime)
	self.m_Velocity = self.m_Velocity + Vect3f(0.0,-10.0,0.0) * ElapsedTime
end

function CPlayerComponent:SetAttackDirection(Value)
	self.m_AttackDirection = Value
end

function CPlayerComponent:Initialize()

	-- local l_GameObject = self.m_LuaGameObject:GetGameObject()
	local l_CColliderName = self.m_LuaGameObject:GetName().."_CharacterCollider"
	
	g_PhysXManager:AddCharacterColliderComponent(l_CColliderName, self.m_LuaGameObject, self.m_Height, self.m_Radius, self.m_Density)
	
	local l_AudioSourceName = self.m_LuaGameObject:GetName().."_AudioSource"
	g_SoundManager:AddComponent(l_AudioSourceName, self.m_LuaGameObject)
	self.m_LuaGameObject:AddSound("SonidoDePrueba","Play_Hit")
	--self.m_AudioSource:AddSound("OpenMapSound","Play_Open_Map")
	
	--Animations
	local l_ACName = self.m_LuaGameObject:GetName().."_AnimatorController"
	g_AnimatorControllerManager:AddComponent(l_ACName, self.m_LuaGameObject)
	
	local l_Idle = self.m_LuaGameObject:AddState("Idle_State", "idle", 1.0, "OnEnter_Idle_Player", "OnUpdate_Idle_Player", "OnExit_Idle_Player")
	local l_Walk = self.m_LuaGameObject:AddState("Walk_State", "walk", 1.0, "OnEnter_Walk_Player", "OnUpdate_Walk_Player", "OnExit_Walk_Player")
	
	local l_AttacksAnimations = stringVector()
	local l_AnimationOne = "normalAttack"
	local l_AnimationTwo = "iraAttack"
	local l_AnimationThree = "die"
	l_AttacksAnimations:push_back(l_AnimationOne)
	l_AttacksAnimations:push_back(l_AnimationTwo)
	l_AttacksAnimations:push_back(l_AnimationThree)
	local l_RestartAnimationsTime = 1.5
	
	--local l_Attack = self.m_LuaGameObject:AddState("Attack_State", l_AttacksAnimations, l_RestartAnimationsTime, 1.0, "OnEnter_Attack_Player", "OnUpdate_Attack_Player", "OnExit_Attack_Player")
	local l_Attack = self.m_LuaGameObject:AddState("Attack_State", "normalAttack", 1.0, "OnEnter_Attack_Player", "OnUpdate_Attack_Player", "OnExit_Attack_Player")
	
	local l_Block = self.m_LuaGameObject:AddState("Block_State", "block", 1.0, "OnEnter_Block_Player", "OnUpdate_Block_Player", "OnExit_Block_Player")
	local l_Injured = self.m_LuaGameObject:AddState("Injured_State", "die", 1.0, "OnEnter_Injured_Player", "OnUpdate_Injured_Player", "OnExit_Injured_Player")
	-- local l_Dead = self.m_LuaGameObject:AddState("Dead_State", "die", 1.0, "OnEnter_Dead_Player", "OnUpdate_Dead_Player", "OnExit_Dead_Player")
	local l_Tossed = self.m_LuaGameObject:AddState("Tossed_State", "idle", 1.0, "OnEnter_Tossed_Player", "OnUpdate_Tossed_Player", "OnExit_Tossed_Player")
	
	self.m_LuaGameObject:AddBool("Walk", false)
	self.m_LuaGameObject:AddTrigger("Attack", false)
	self.m_LuaGameObject:AddTrigger("Block", false)
	self.m_LuaGameObject:AddTrigger("TossedByDragon", false)
	
	local l_IdleToWalk = l_Idle:AddTransition("IdleToWalk", l_Walk, false, 0.1)
	l_IdleToWalk:AddBoolCondition("Walk", true)
	
	local l_WalkToIdle = l_Walk:AddTransition("WalkToIdle", l_Idle, false, 0.1)
	l_WalkToIdle:AddBoolCondition("Walk", false)
	
	local l_IdleToBlock = l_Idle:AddTransition("IdleToBlock", l_Block, false, 0.1, 0.2)
	l_IdleToBlock:AddTriggerCondition("Block")
	
	local l_IdleToAttack = l_Idle:AddTransition("IdleToAttack", l_Attack, false, 1.7, 0.8)
	l_IdleToAttack:AddTriggerCondition("Attack")
	local l_AttackToIdle = l_Attack:AddTransition("AttackToIdle", l_Idle, true, 0.2)
	
	local l_WalkToAttack = l_Walk:AddTransition("l_WalkToAttack", l_Attack, false, 0.1, 0.1)
	l_WalkToAttack:AddTriggerCondition("Attack")
	
	local l_BlockToIdle = l_Block:AddTransition("BlockToIdle", l_Idle, true, 0.1)
	
	local l_IdleToTossed = l_Idle:AddTransition("IdleToTossed", l_Tossed, false, 0.2)
	l_IdleToTossed:AddTriggerCondition("TossedByDragon")
	local l_TossedToIdle = l_Tossed:AddTransition("TossedToIdle", l_Idle, true, 0.2)

	local l_WalkToTossed = l_Walk:AddTransition("WalkToTossed", l_Tossed, false, 0.2)
	l_WalkToTossed:AddTriggerCondition("TossedByDragon")
	
	-- local l_IdleToBlock = l_Idle:AddTransition("IdleToBlock", l_Block, true, 0.5, 1.0)
	-- l_IdleToBlock:AddTriggerCondition("Block")

	self:InitializePlayerStats()
end

function CPlayerComponent:InitializePlayerStats()

	local l_HealthPotion = CHealthPotion(4.0,50.0)
	local l_ButtonGuiPosition = SGUIPosition(0.95, 0.85, 0.08, 0.070, CGUIManager.TOP_CENTER, CGUIManager.GUI_RELATIVE, CGUIManager.GUI_RELATIVE_WIDTH)
	local l_TextGuiPosition = SGUIPosition(0.968, 0.785, 0.08, 0.070, CGUIManager.TOP_CENTER, CGUIManager.GUI_RELATIVE, CGUIManager.GUI_RELATIVE_WIDTH)
	l_HealthPotion:AddButton("health_potion_0", "health_potion", "health_potion_normal", "health_potion_highlight", "health_potion_pressed",CColor(1.0,1.0,1.0,1.0))
	l_HealthPotion:AddCooldownButton("health_potion_0", "health_potion", "health_potion_normal", "health_potion_highlight", "health_potion_pressed",CColor(0.6,1.0,1.0,0.85))
	l_HealthPotion:AddText("health_potion_units", "health_potion_font", "Data\\GUI\\Fonts\\health_potion_font.fnt", "health_potion_font_0.png", "")
	
	self.m_Inventory:AddItem(l_HealthPotion,5)
end 

dofile("./Data/Scripting/Player/PlayerController.lua")

function CPlayerComponent:Update(ElapsedTime)
	self.m_Inventory:Update(ElapsedTime)
	self:PlayerController(ElapsedTime)
	self:HandleEvents()
	--g_LogManager:Log("CPlayerComponent updated")
end

function CPlayerComponent:HandleEvents()
	if self:FullHealth() then
		g_EventManager:FireEvent("PlayerHasFullHealth")
	end
end

function CPlayerComponent:Lock()
	self.m_Locked = true
end
function CPlayerComponent:Unlock()
	self.m_Locked = false
end
function CPlayerComponent:IsLocked()
	return self.m_Locked
end

function CPlayerComponent:IsBeingTossed(Value) self.m_BeeingTossed = Value end

function CPlayerComponent:Health(Health) 
	if((self.m_Health + Health)>self.m_MaxHealth) then
		self.m_Health = self.m_MaxHealth
	else
		self.m_Health = self.m_Health + Health 
	end
end

function CPlayerComponent:FullHealth()
	return (self.m_Health==self.m_MaxHealth)
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

function CPlayerComponent:IsBlocking() return self.m_IsBlocking end
function CPlayerComponent:SetBlockingState(state) self.m_IsBlocking = state end

dofile("./Data/Scripting/Player/PlayerCombatMechanics.lua")

function CPlayerComponent:OpenMap()
	self.m_MapOpened = not self.m_MapOpened
	if (self.m_MapOpened) then
		g_Engine:SetTimeScale(0.0)
	else
		g_Engine:SetTimeScale(1.0)
	end
	--self.m_AudioSource:PlayEvent("OpenMapSound")
end

function CPlayerComponent:DrinkBeer()
	self.m_Inventory:UseItem(1)
end
