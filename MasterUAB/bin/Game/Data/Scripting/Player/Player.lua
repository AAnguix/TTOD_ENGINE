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
	self.m_MaxHealth=150.0
	self.m_Health=150.0
	self.m_Dead = false
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
	
	self.m_RotationVelocity = 4.0
	self.m_AngleMargin = 0.05
	self.m_AttackDirection = Vect3f(0.0,0.0,0.0)
	self.m_AttackDisplacement = 0.7
	
	self.m_Velocity = Vect3f(0.0,0.0,0.0)
	self.m_Speed = 3.0
	
	--Other
	self.m_MapOpened = false
	self.m_IsBlocking = false
	self.m_Attacking = false
	self.m_AttackFinished = true
	self.m_BeeingTossed = false
	
	self.m_DistanceToEnterCombatIdle = 5.0
	
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

dofile("./Data/Scripting/Player/PlayerInitialize.lua")

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
	if (self.m_Dead) then
		self:CountdownToExtintion(ElapsedTime)
	end
	--g_LogManager:Log("CPlayerComponent updated")
end

function CPlayerComponent:Die()
	self.m_Dead = true
	self:Lock()
	g_EventManager:FireEvent("CENTER_CAMERA")
	g_Engine:SetTimeScale(0.7)
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
	end
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
