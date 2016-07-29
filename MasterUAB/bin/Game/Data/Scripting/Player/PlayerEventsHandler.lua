--EVENTS

function CPlayerComponent:PLAYER_OPENS_MAP() self:OpenMap() end
function CPlayerComponent:PLAYER_ATTACKS() if self:IsAttackFinished() then self.m_LuaGameObject:SetTrigger("Attack") end end
function CPlayerComponent:PLAYER_BLOCKS() self.m_LuaGameObject:SetTrigger("Block") end
function CPlayerComponent:PLAYER_DRINKS_BEER() self:DrinkBeer() end
function CPlayerComponent:LOCK_CHARACTER()
	if (self:IsLocked()) then	
		self:Unlock()
	else
		self:Lock()
	end
end

function CPlayerComponent:PLAYER_WALKING_FORWARD()
	if self:IsAttacking()==false then
		self.m_Forward = true
		self.m_Walk = true
		local l_CameraForward = g_CameraControllerManager:GetCurrentCameraController():GetForward()
		self:AddVelocity(l_CameraForward)
	end
end

function CPlayerComponent:PLAYER_WALKING_BACKWARDS()
	if self:IsAttacking()==false then
		self.m_Backwards = true
		self.m_Walk = true
		local l_CameraForward = g_CameraControllerManager:GetCurrentCameraController():GetForward()
		self:AddVelocity(l_CameraForward*(-1))
	end
end

function CPlayerComponent:PLAYER_WALKING_RIGHT()
	if self:IsAttacking()==false then
		self.m_Right = true
		self.m_Walk = true
		local l_CameraRight = g_CameraControllerManager:GetCurrentCameraController():GetRight()
		self:AddVelocity(l_CameraRight*(-1))
	end
end

function CPlayerComponent:PLAYER_WALKING_LEFT()
	if self:IsAttacking()==false then
		self.m_Left = true
		self.m_Walk = true
		local l_CameraRight = g_CameraControllerManager:GetCurrentCameraController():GetRight()
		self:AddVelocity(l_CameraRight)
	end
end