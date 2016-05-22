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
function CPlayerComponent:__init()
	CLUAComponent.__init(self,"player")
	self.m_Health=500.0
	self.m_Speed=1.0
	self.m_AttackDelay = 1.0
	self.m_CharacterControllerHeight = 1.4
	self.m_Armors={}
	self.m_Weapons={}
	self.m_CurrentArmor = nil
	self.m_CurrentWeapon = nil
	self.m_Inventory = CInventory()
end

function CPlayerComponent:Initialize(CAnimatedInstance)

--Physyx
	local l_CharacterCollider = CCharacterCollider.AddCharacterCollider("CharacterCollider", CAnimatedInstance)
	if l_CharacterCollider ~= nil then 
		local l_Material = l_CharacterCollider:GetPhysxMaterial()
		local l_MaterialName = l_Material:GetName()
		g_PhysXManager:RegisterMaterial(l_MaterialName, l_Material:GetStaticFriction(), l_Material:GetDynamicFriction(), l_Material:GetRestitution())
		m_Position = CAnimatedInstance:GetPosition()
		l_CControlerPos = Vect3f(m_Position.x, m_Position.y, m_Position.z)
		g_PhysXManager:CreateCharacterController(CAnimatedInstance:GetName(), self.m_CharacterControllerHeight, 0.3, 30.0, l_CControlerPos, l_MaterialName)
	end
	
	--Animations
	CAnimatorController.AddAnimatorController("AnimatorController", CAnimatedInstance)

	local l_Idle = CAnimatedInstance:GetAnimatorController():AddState("Idle_State", "idle", 1.0, "OnEnter_Idle_Player", "OnUpdate_Idle_Player", "OnExit_Idle_Player")
	local l_Walk = CAnimatedInstance:GetAnimatorController():AddState("Walk_State", "walk", 1.0, "OnEnter_Walk_Player", "OnUpdate_Walk_Player", "OnExit_Walk_Player")
	local l_Attack = CAnimatedInstance:GetAnimatorController():AddState("Attack_State", "normalAttack", 1.0, "OnEnter_Attack_Player", "OnUpdate_Attack_Player", "OnExit_Attack_Player")
	local l_Block = CAnimatedInstance:GetAnimatorController():AddState("Block_State", "die", 1.0, "OnEnter_Block_Player", "OnUpdate_Block_Player", "OnExit_Block_Player")

	--GetAnimatorController()->AddBool("Run", false);
	CAnimatedInstance:GetAnimatorController():AddBool("Walk", false)
	CAnimatedInstance:GetAnimatorController():AddTrigger("Attack", false)
	CAnimatedInstance:GetAnimatorController():AddTrigger("Block", false)

	local l_IdleToWalk = l_Idle:AddTransition("IdleToWalk", l_Walk, false, 0.0, 0.1, 0.2)
	l_IdleToWalk:AddBoolCondition("Walk", true)

	local l_WalkToIdle = l_Walk:AddTransition("WalkToIdle", l_Idle, false, 0.0, 0.1, 0.2)
	l_WalkToIdle:AddBoolCondition("Walk", false)

	local l_IdleToAttack = l_Idle:AddTransition("IdleToAttack", l_Attack, false, 0.0, 0.1, 0.25)
	l_IdleToAttack:AddTriggerCondition("Attack")

	local l_IdleToBlock = l_Idle:AddTransition("IdleToBlock", l_Block, true, 0.0, 0.5, 1.0)
	l_IdleToBlock:AddTriggerCondition("Block")

	self:InitializePlayerStats()
	g_LogManager:Log("Player initialized...")

end

function CPlayerComponent:InitializePlayerStats()

	local l_HealthPotion = CHealthPotion(15.0,50.0)
	local l_ButtonGuiPosition = SGUIPosition(0.95, 0.85, 0.08, 0.070, CGUIManager.TOP_CENTER, CGUIManager.GUI_RELATIVE, CGUIManager.GUI_RELATIVE_WIDTH)
	local l_TextGuiPosition = SGUIPosition(0.5, 0.5, 0.08, 0.070, CGUIManager.TOP_CENTER, CGUIManager.GUI_RELATIVE, CGUIManager.GUI_RELATIVE_WIDTH)
	l_HealthPotion:AddButton("health_potion_0", "health_potion", "health_potion_normal", "health_potion_highlight", "health_potion_pressed", l_ButtonGuiPosition)
	l_HealthPotion:AddText("health_potion_units", "health_potion_font", "Data\\GUI\\Fonts\\health_potion_font.fnt", l_TextGuiPosition, "health_potion_font_sprite",  "aaaa")
	
	-- self.m_Inventory:AddItem(l_HealthPotion,5)
end 

function CPlayerComponent:Update(ElapsedTime)
	self.m_Inventory:Update(ElapsedTime)
end

function CPlayerComponent:GetHealth()
	return self.m_Health
end
function CPlayerComponent:GetSpeed()
	return self.m_Speed
end

function CPlayerComponent:AddWeapon(CArmorComponent)
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
function CPlayerComponent:ChangeArmor(Index)
	self.m_CurrentArmor = m_Armors[Index]
end
function CPlayerComponent:ChangeWeapon(Index)
	self.m_CurrentWeapon = m_Weapons[Index]
end
function CPlayerComponent:GetArmor(Index)
	return m_Armors[Index]
end
function CPlayerComponent:GetWeapon(Index)
	return m_Weapons[Index]
end

function CPlayerComponent:TakeDamage(EnemyWeapon, EnemyDamage)
	self.m_Health = self.m_Health - Damage
end

function CPlayerComponent:GetType()
	return self:GetSpeed()
end

function CPlayerComponent:GetReference()
	return self
end

--loadstring(string_s)()