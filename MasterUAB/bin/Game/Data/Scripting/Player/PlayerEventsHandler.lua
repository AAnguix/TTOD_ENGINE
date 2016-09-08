--EVENTS

function CPlayerComponent:PLAYER_OPENS_MAP() 
	if(g_Engine:IsPaused()==false) then 
		self:OpenMap() 
	end 
end
function CPlayerComponent:PLAYER_ATTACKS() 
	if (not self:IsLocked()) and self:IsAttackFinished() then 
		self.m_LuaGameObject:SetTrigger("Attack") 
	end
end
function CPlayerComponent:PLAYER_BLOCKS()
	if (not self:IsLocked()) then 
		self.m_LuaGameObject:SetTrigger("Block") 
	end 
end
 
function CPlayerComponent:PLAYER_DRINKS_BEER() self:DrinkBeer() end

function CPlayerComponent:LOCK_CHARACTER()
	if (self:IsLocked()) then	
		self:Unlock()
	else
		self:Lock()
	end
end

function CPlayerComponent:CanMove()
	-- local l_Result = (not self:IsAttacking()) and (not self:IsBlocking()) 
	-- and (not self:IsInteracting()) and (not self:IsLocked())
	return (not self:IsLocked())
end

function CPlayerComponent:PLAYER_WALKING_FORWARD()
	if self:CanMove() then
		self.m_Forward = true
		--self.m_Walk = true
		local l_CameraForward = g_CameraControllerManager:GetCurrentCameraController():GetForward()
		self:AddVelocity(l_CameraForward)
	end
end

function CPlayerComponent:PLAYER_WALKING_BACKWARDS()
	if self:CanMove() then
		self.m_Backwards = true
		--self.m_Walk = true
		local l_CameraForward = g_CameraControllerManager:GetCurrentCameraController():GetForward()
		self:AddVelocity(l_CameraForward*(-1))
	end
end

function CPlayerComponent:PLAYER_WALKING_RIGHT()
	if self:CanMove() then
		self.m_Right = true
		--self.m_Walk = true
		local l_CameraRight = g_CameraControllerManager:GetCurrentCameraController():GetRight()
		self:AddVelocity(l_CameraRight*(-1))
	end
end

function CPlayerComponent:PLAYER_WALKING_LEFT()
	if self:CanMove() then
		self.m_Left = true
		--self.m_Walk = true
		local l_CameraRight = g_CameraControllerManager:GetCurrentCameraController():GetRight()
		self:AddVelocity(l_CameraRight)
	end
end