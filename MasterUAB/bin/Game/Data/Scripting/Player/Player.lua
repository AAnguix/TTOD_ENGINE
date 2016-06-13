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
function CPlayerComponent:__init(CGameObject)
	CLUAComponent.__init(self,CGameObject:GetName().."_PlayerScript")
	self.m_GameObject = CGameObject
	self.m_RObject = CGameObject:GetRenderableObject()
	self.m_MaxHealth=500.0
	self.m_Health=self.m_MaxHealth
	self.m_Speed=1.0
	self.m_AttackDelay = 1.0
	self.m_CharacterControllerHeight = 1.4
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
	
	--Components
	self.m_Animator = nil 
	self.m_AudioSource = nil
end

function CPlayerComponent:Initialize()

	local l_CColliderName = self.m_GameObject:GetName().."_CharacterCollider"
	local l_CharacterCollider = g_PhysXManager:AddCharacterColliderComponent(l_CColliderName, self.m_GameObject)
	if l_CharacterCollider ~= nil then 
		local l_Material = l_CharacterCollider:GetPhysxMaterial()
		local l_MaterialName = l_Material:GetName()
		m_Position = self.m_RObject:GetPosition()
		l_CControlerPos = Vect3f(m_Position.x, m_Position.y, m_Position.z)
		g_PhysXManager:CreateCharacterController(self.m_GameObject:GetName(), self.m_CharacterControllerHeight, 0.3, 30.0, l_CControlerPos, l_MaterialName, l_Material:GetStaticFriction(), l_Material:GetDynamicFriction(), l_Material:GetRestitution())
	end
	
	local l_AudioSourceName = self.m_GameObject:GetName().."_AudioSource"
	local l_AudioSource = g_SoundManager:AddComponent(l_AudioSourceName, self.m_GameObject)
	if l_AudioSource ~= nil then  
		self.m_AudioSource = l_AudioSource
		self.m_AudioSource:AddSound("SonidoDePrueba","Play_Hit")
	end

	--Animations
	local l_ACName = self.m_GameObject:GetName().."_AnimatorController"
	self.m_Animator = g_AnimatorControllerManager:AddComponent(l_ACName, self.m_GameObject)
	
	local l_Idle = self.m_Animator:AddState("Idle_State", "idle", 1.0, "OnEnter_Idle_Player", "OnUpdate_Idle_Player", "OnExit_Idle_Player")
	local l_Walk = self.m_Animator:AddState("Walk_State", "walk", 1.0, "OnEnter_Walk_Player", "OnUpdate_Walk_Player", "OnExit_Walk_Player")
	local l_Attack = self.m_Animator:AddState("Attack_State", "normalAttack", 1.0, "OnEnter_Attack_Player", "OnUpdate_Attack_Player", "OnExit_Attack_Player")
	local l_Block = self.m_Animator:AddState("Block_State", "die", 1.0, "OnEnter_Block_Player", "OnUpdate_Block_Player", "OnExit_Block_Player")
	local l_Rotate = self.m_Animator:AddState("Rotate_State", "walk", 1.0, "OnEnter_Rotate_Player", "OnUpdate_Rotate_Player", "OnExit_Rotate_Player")
	
	--GetAnimatorController()->AddBool("Run", false);
	self.m_Animator:AddBool("Walk", false)
	self.m_Animator:AddBool("Rotate", false)
	self.m_Animator:AddTrigger("Attack", false)
	self.m_Animator:AddTrigger("Block", false)
	
	
	local l_IdleToWalk = l_Idle:AddTransition("IdleToWalk", l_Walk, false, 0.0, 0.1, 0.2)
	l_IdleToWalk:AddBoolCondition("Walk", true)
	
	-- local l_IdleToRotate = l_Idle:AddTransition("IdleToRotate", l_Rotate, false, 0.0, 0.1, 0.2)
	-- l_IdleToRotate:AddBoolCondition("Rotate",true)
	
	-- local l_RotateToIdle = l_Rotate:AddTransition("RotateToIdle", l_Idle, true, 0.0, 0.1, 0.2)
	-- l_RotateToIdle:AddBoolCondition("Rotate",false)
	
	
	-- local l_RotateToWalk = l_Rotate:AddTransition("RotateToWalk", l_Walk, false, 0.0, 0.1, 0.2)
	-- l_RotateToWalk:AddBoolCondition("Walk", true)
	-- l_RotateToWalk:AddBoolCondition("Rotate",false)
	
	local l_WalkToIdle = l_Walk:AddTransition("WalkToIdle", l_Idle, false, 0.0, 0.1, 0.2)
	l_WalkToIdle:AddBoolCondition("Walk", false)
	
	
	-- local l_WalkToIdle = l_Walk:AddTransition("IdleToRotate", l_Idle, false, 0.0, 0.1, 0.2)
	-- l_WalkToIdle:AddBoolCondition("Walk", false)
	
	-- local l_RotateToAttack = l_Rotate:AddTransition("RotateToAttack", l_Attack, false, 0.0, 0.1, 0.2)
	-- l_RotateToAttack:AddTriggerCondition("Attack")
	-- l_RotateToAttack:AddTriggerCondition("Rotate")
	
	-- local l_IdleToBlock = l_Idle:AddTransition("IdleToBlock", l_Block, true, 0.0, 0.5, 1.0)
	-- l_IdleToBlock:AddTriggerCondition("Block")

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

dofile("./Data/Scripting/PlayerController.lua")

function CPlayerComponent:Update(ElapsedTime)
	self.m_Inventory:Update(ElapsedTime)
	self:PlayerController(ElapsedTime)
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

function CPlayerComponent:GetHealth() return self.m_Health end
function CPlayerComponent:GetMaxHealth() return self.m_MaxHealth end
function CPlayerComponent:GetSpeed() return self.m_Speed end
function CPlayerComponent:GetReference() return self end

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
function CPlayerComponent:ChangeArmor(Index) self.m_CurrentArmor = m_Armors[Index] end
function CPlayerComponent:ChangeWeapon(Index) self.m_CurrentWeapon = m_Weapons[Index] end
function CPlayerComponent:GetArmor(Index) return m_Armors[Index] end
function CPlayerComponent:GetWeapon(Index) return m_Weapons[Index] end

function CPlayerComponent:TakeDamage(EnemyWeapon, EnemyDamage)
	self.m_AudioSource:PlayEvent("SonidoDePrueba")
	--local l_Armor = self.m_CurrentArmor:GetType()
	local l_Armor = "heroic"
	local l_DamageCalculated = g_DamageCalculator:CalculateDamage(l_Armor,EnemyWeapon,EnemyDamage)
	if self.m_Health >= 0.0 then
		self.m_Health = self.m_Health - l_DamageCalculated
	end
end

function CPlayerComponent:GetClosestEnemy(Enemies)
	local l_ClosestEnemy = nil
	local l_MinDistance = 0.0
	for i=1, (#Enemies) do
		local l_EnemyPos = Enemies[i]:GetRenderableObject():GetPosition()
		local l_Distance = (l_EnemyPos-self.m_RObject:GetPosition()):Length()
		if ((l_MinDistance==0.0)or(l_Distance <= l_MinDistance)) then
			l_ClosestEnemy = Enemies[i]
			l_MinDistance = l_Distance
		end
	end
	return l_ClosestEnemy
end

function CPlayerComponent:FaceEnemy(Enemies,ElapsedTime)
	local l_ClosestEnemy = self:GetClosestEnemy(Enemies)
	local l_Forward = self.m_RObject:GetForward()
	local l_EnemyPos = l_ClosestEnemy:GetRenderableObject():GetPosition()
	local l_Angle = CTTODMathUtils.GetAngleToFacePoint(l_Forward, self.m_RObject:GetPosition(), l_EnemyPos)	
	
	local l_CurrentYaw = self.m_RObject:GetYaw()
	local l_Velocity = self.m_RotationVelocity
	self.m_RObject:SetYaw(CTTODMathUtils.CalculateNewAngle(l_Angle, l_CurrentYaw, l_Velocity, ElapsedTime))
end

function CPlayerComponent:FaceAttackDirection(ElapsedTime)
	
	--self.m_RObject:SetYaw(g_CameraControllerManager:GetCurrentCameraController():GetYaw())
	local l_CurrentYaw = self.m_RObject:GetYaw()
	local l_Angle = g_CameraControllerManager:GetCurrentCameraController():GetYaw() - l_CurrentYaw
	self.m_RObject:SetYaw(CTTODMathUtils.CalculateNewAngle(l_Angle, l_CurrentYaw, self.m_RotationVelocity, ElapsedTime))
	
	-- local l_Forward = self.m_RObject:GetForward()
	-- local l_CameraForward = g_CameraControllerManager:GetCurrentCameraController():GetForward()
	
	-- local l_Angle = CTTODMathUtils.AngleBetweenVectors(l_Forward,l_CameraForward)	
	
	-- local l_CurrentYaw = self.m_RObject:GetYaw()
	-- local l_Velocity = self.m_RotationVelocity
	-- self.m_RObject:SetYaw(CTTODMathUtils.CalculateNewAngle(l_Angle, l_CurrentYaw, l_Velocity, ElapsedTime))
end
