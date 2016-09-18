function CThrowingWeaponComponent:UpdateProjectileParticles(ElapsedTime)
	if self.m_ProjectileParticlesEnabled then
		self:AddTime(ElapsedTime)
		if(self:GetTimer()>self.m_ProjectileParticlesDuration) then
			self:EnableProjectileParticles(false)
		else
			if (self.m_ProjectileParticlesLuaGameObject ~= nil)then
				self.m_ProjectileParticlesLuaGameObject:SetPosition(self.m_LuaGameObject:GetPosition())
            end 
		end
	end
end

function CThrowingWeaponComponent:UpdateOnPlayerContactParticles(ElapsedTime)
	if self.m_OnPlayerContactParticlesEnabled then
		self:UpdateOnPlayerContactParticlesTimer(ElapsedTime)
		if(self.m_OnPlayerContactParticlesTimer>self.m_OnPlayerContactParticlesDuration) then
			self:EnableOnPlayerContactParticles(false)
			self:ResetOnPlayerContactParticlesTimer()
		end
	end
end

function CThrowingWeaponComponent:UpdateOnPlayerContactParticlesTimer(ElapsedTime)
	self.m_OnPlayerContactParticlesTimer = self.m_OnPlayerContactParticlesTimer + ElapsedTime
end
function CThrowingWeaponComponent:ResetOnPlayerContactParticlesTimer()
	self.m_OnPlayerContactParticlesTimer = 0.0
end

function CThrowingWeaponComponent:EnableProjectileParticles(Value)
	self.m_ProjectileParticlesEnabled = Value
	self.m_ProjectileParticlesLuaGameObject:EnableRenderableObject(Value)
	if Value == false then
		self:ResetTimer()
	end
end

function CThrowingWeaponComponent:EnableOnPlayerContactParticles(Value)
	self.m_OnPlayerContactParticlesEnabled = Value
	self.m_OnPlayerContactParticlesLuaGameObject:EnableRenderableObject(Value)
end