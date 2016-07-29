class 'CWeaponComponent' (CLUAComponent)
function CWeaponComponent:__init(ComponentType, ParentLuaGameObject, ParentBoneName, LuaGameObject, Damage, WeaponType)
	CLUAComponent.__init(self,ComponentType)
	self.m_LuaGameObject = LuaGameObject
	self.m_ParentLuaGameObject = ParentLuaGameObject --The A.Model that is going to use this weapon
	self.m_Damage = Damage
	self.m_Type = WeaponType
	self.m_BoneID = self.m_ParentLuaGameObject:GetAnimatedCoreModel():GetBoneId(ParentBoneName)
	self.m_LuaGameObject:SetParent(self.m_ParentLuaGameObject,ParentBoneName)
	self.m_BasePosition = self.m_LuaGameObject:GetPosition()
	--g_LogManager:Log(self.m_BasePosition)
	self.m_BaseYaw = self.m_LuaGameObject:GetYaw()
	--g_LogManager:Log("BSeYaw "..self.m_BaseYaw)
	self.m_BasePitch = self.m_LuaGameObject:GetPitch()
	--g_LogManager:Log("BPitch "..self.m_BasePitch)
	self.m_BaseRoll = self.m_LuaGameObject:GetRoll()
	--g_LogManager:Log("BRoll "..self.m_BaseRoll)
	--g_LogManager:Log("Weapon created")
end

function CWeaponComponent:GetLuaGameObject() return self.m_LuaGameObject end

-- function CWeaponComponent:Initialize(ParentBoneName)
	
-- end

function CWeaponComponent:Update(ElapsedTime)
	self:AddTime(ElapsedTime)
	-- if self:GetTimer() >= 0.1 then
		self:SetWeaponTransform(ElapsedTime)
	
		-- self:ResetTimer()
	-- end
	--g_LogManager:Log("Actualizando arma")
end

function CWeaponComponent:OnTriggerEnter(Actor)
	--g_LogManager:Log("OnTriggerEnter "..self.m_LuaGameObject:GetName()..". Actor:"..Actor)
	local l_ParentName = self.m_ParentLuaGameObject:GetName()
	
	if (l_ParentName=="Player") then
		if Actor ~= "Player"  then
			local l_AttackingState = g_PlayerComponent:IsAttacking()
			if l_AttackingState then
				local l_Enemy = g_GameController:GetEnemy(Actor)
				if l_Enemy ~=  nil then
					l_Enemy:TakeDamage(self)
					l_Enemy:SetHitState(true)
					g_PlayerComponent:SetAttacking(false) 
				end
			end
		end
	else --Parent is an enemy
		local l_Parent = g_GameController:GetEnemy(l_ParentName)
		local l_AttackingState = l_Parent:IsAttacking()
		if l_AttackingState then
			if g_PlayerComponent:IsBlocking() then
				local l_Angle = CTTODMathUtils.AngleBetweenVectors(g_Player:GetForward(),l_Parent:GetLuaGameObject():GetForward())
				math.abs(l_Angle)
				g_LogManager:Log(l_Angle.." Angulos")
				if (l_Angle > g_PlayerComponent:GetMinBlockAngle() and l_Angle < g_PlayerComponent:GetMaxBlockAngle()) then
					--g_LogManager:Log("Ataque bloqueado")
					l_Parent:SetAttacking(false)  --Para que no entre mas de 1 vez por ataque, Verificar cunado las fisicas se muevan bien
				else
					--g_LogManager:Log("Bloqueo Levantado pero el ataque Pego")
					g_PlayerComponent:TakeDamage(self)
					l_Parent:SetAttacking(false)  --Para que no entre mas de 1 vez por ataque, Verificar cunado las fisicas se muevan bien
				end
			else
				--g_LogManager:Log("Choque Trigger y player con el flag Attackng levantado")
				g_PlayerComponent:TakeDamage(self)
				l_Parent:SetAttacking(false)  --Para que no entre mas de 1 vez por ataque, Verificar cunado las fisicas se muevan bien
			end
			
		else
			--g_LogManager:Log("Choca trigger y player pero no se estan atacando")
		end
	end	
end

function CWeaponComponent:OnTriggerExit(Object)
	--g_LogManager:Log("OnTriggerExit "..self.m_LuaGameObject:GetName()..". Actor:"..Object)
end

function CWeaponComponent:SetWeaponTransform(ElapsedTime)
	local l_BoneTransform = Mat44f()
	local l_ParentTransform = Mat44f()
	l_BoneTransform = self.m_ParentLuaGameObject:GetBoneTransformationMatrix(self.m_BoneID)
	l_ParentTransform = self.m_ParentLuaGameObject:GetTransform()
	
	local l_Transform = Mat44f()
	l_Transform = self:Get3DMaxTransform()
	local l_Matrix = Mat44f()
	l_Matrix = l_Transform*l_BoneTransform*l_ParentTransform
	local l_Position = Vect3f(0.0,0.0,0.0)
	
	l_Position = l_Matrix:GetWorldPos()
	local l_Yaw = l_Matrix:GetYaw()
	local l_Pitch = l_Matrix:GetPitch()
	local l_Roll = l_Matrix:GetRoll()
	-- g_LogManager:Log("ESTO EN LUA")
	-- g_LogManager:Log(l_Position)	
	-- g_LogManager:Log(l_Yaw.."")	
	-- g_LogManager:Log(l_Pitch.."")	
	-- g_LogManager:Log(l_Roll.."")	
	local l_Quat = Quatf(l_Yaw, l_Pitch+(0.3), l_Roll)
	g_PhysXManager:MoveKinematicActor(self.m_LuaGameObject:GetName(), l_Position, l_Quat)
end

function CWeaponComponent:Get3DMaxTransform()

	local l_Yaw = self.m_LuaGameObject:GetYaw()
	local l_Pitch = self.m_LuaGameObject:GetPitch()
	local l_Roll = self.m_LuaGameObject:GetRoll()

	local l_RotX = Mat44f()
	local l_RotY = Mat44f()
	local l_RotZ = Mat44f()
	local l_Translation = Mat44f()
	l_Translation:SetIdentity()
	--l_Translation:Translate(self.m_BasePosition)
	l_Translation:Translate(self.m_LuaGameObject:GetPosition())
	
	l_RotX:SetIdentity()
	l_RotX:RotByAngleX(l_Pitch)
	
	l_RotY:SetIdentity()
	l_RotY:RotByAngleY(l_Yaw)
	
	l_RotZ:SetIdentity()
	l_RotZ:RotByAngleZ(l_Roll)
	
	local l_Tranform = Mat44f()
	l_Tranform = l_RotX*l_RotY*l_RotZ*l_Translation
	return l_Tranform

end

function CWeaponComponent:GetDamage()
	return self.m_Damage
end

function CWeaponComponent:GetType()
	return self.m_Type
end

function CWeaponComponent:GetParentLuaGameObject()
	return self.m_ParentLuaGameObject
end

