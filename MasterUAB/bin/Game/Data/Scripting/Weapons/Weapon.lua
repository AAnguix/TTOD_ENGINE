class 'CWeaponComponent' (CLUAComponent)
function CWeaponComponent:__init(ComponentType, ParentLuaGameObject, ParentBoneName, LuaGameObject, Damage, WeaponType, WeaponStyle)
	CLUAComponent.__init(self,ComponentType)
	self.m_LuaGameObject = LuaGameObject
	self.m_ParentLuaGameObject = ParentLuaGameObject --The A.Model that is going to use this weapon
	self.m_Damage = Damage
	self.m_Type = WeaponType
	self.m_Style = WeaponStyle
	self.m_BoneID = self.m_ParentLuaGameObject:GetAnimatedCoreModel():GetBoneId(ParentBoneName)
	--self.m_LuaGameObject:SetParent(self.m_ParentLuaGameObject,ParentBoneName)
	self.m_BasePosition = self.m_LuaGameObject:GetPosition()
	
	self.m_BaseYaw = self.m_LuaGameObject:GetYaw()
	
	self.m_BasePitch = self.m_LuaGameObject:GetPitch()
	
	self.m_BaseRoll = self.m_LuaGameObject:GetRoll()
end

function CWeaponComponent:Update(ElapsedTime)
end

function CWeaponComponent:PlayerAttacksEnemy(EnemyActor)
	local l_AttackingState = g_PlayerComponent:IsAttacking()
	if l_AttackingState then
		local l_Enemy = g_GameController:GetEnemy(EnemyActor)
		if l_Enemy ~=  nil then
			l_Enemy:TakeDamage(self)
			g_PlayerComponent:SetAttacking(false) 
		end
	end
end

function CWeaponComponent:OnTriggerEnter(Actor)
end

function CWeaponComponent:OnTriggerExit(Object)
end

function CWeaponComponent:GetLuaGameObject() return self.m_LuaGameObject end
function CWeaponComponent:GetDamage() return self.m_Damage end
function CWeaponComponent:GetType()	return self.m_Type end
function CWeaponComponent:GetStyle() return self.m_Style end