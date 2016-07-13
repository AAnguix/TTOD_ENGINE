class 'CEnergyBeamEmitter' (CLUAComponent)
function CEnergyBeamEmitter:__init(CGameObject, Plane)
CLUAComponent.__init(self,"CEnergyBeamEmitter")	
	self.m_Used = false
	self.m_GameObject = CGameObject
	self.m_Cooldown = Cooldown
	self.m_CurrentCooldown = 0.0
	self.m_InteractionRange = 1.0
	self.m_EmissionTime = 3.0
	self.m_EmissionDelay = 1.5
	
	self.m_Plane = Plane
	self.m_Ready = false
	self.m_Active = false
	self.m_Emitting = false
	self.m_EmitSoundPlayed = false
	
	local l_AudioSourceName = self.m_GameObject:GetName().."_AudioSource"
	local l_AudioSource = g_SoundManager:AddComponent(l_AudioSourceName, self.m_GameObject)
	if l_AudioSource ~= nil then  
		self.m_AudioSource = l_AudioSource
		--self.m_AudioSource:AddSound("EmitBeam","Play_Emit_Beam")
		--self.m_AudioSource:AddSound("EnergyBeamEmitterReady","Play_Energy_Beam_Emitter_Ready")
	--self.m_AudioSource:AddSound("EnergyBeamEmitterActivatable","Play_Energy_Beam_Emitter_Activatable")
	end
end

function CEnergyBeamEmitter:IsActive() return self.m_Active end
function CEnergyBeamEmitter:Activate(Value) self.m_Active=Value end

function CEnergyBeamEmitter:Update(ElapsedTime)
	if (self.m_Used == false) then
		self:UpdateCooldown(ElapsedTime)
		local l_Position = self.m_GameObject:GetRenderableObject():GetPosition()
		local l_PlayerPos = g_Player:GetRenderableObject():GetPosition()
		if (CTTODMathUtils.PointInsideCircle(l_PlayerPos, l_Position, self.m_InteractionRange)) then
			self:Activatable()
			if((self.m_Ready)and(CInputManager.GetInputManager():IsActionActive("E_PRESSED")))
				self:Activate(true)
			end
		end
		if(self:IsActive()) 
			self:Emit()
		end
		
	end
end

function CEnergyBeamEmitter:Emit()
	self:AddTime(ElapsedTime)
	if((self.m_Emitting==false) and (self:GetTimer() >= self.m_EmissionDelay)) then
		self:ResetTimer()
		self.m_Emitting = true
	end
	if((self.m_Emitting) and (self:GetTimer() <= self.m_EmissionTime)) then
		m_Plane:GetRenderableObject():SetVisible(true)
		if (self.m_EmitSoundPlayed == false) 
			--self.m_AudioSource:PlayEvent("EmitBeam") 
			self.m_EmitSoundPlayed = true
		end
	end
end

function CEnergyBeamEmitter:UpdateCooldown(ElapsedTime)
	if (self.m_CurrentCooldown>0.00) then
		self.m_CurrentCooldown = self.m_CurrentCooldown - ElapsedTime
	end
	if self.m_CurrentCooldown <= 0.0 then
		self:Ready()
	end
end



function CEnergyBeamEmitter:ResetCooldown()
	self.m_CurrentCooldown = self.m_Cooldown
end

function CEnergyBeamEmitter:IsInCooldown()
	if self.m_CurrentCooldown > 0.0 then
		return true
	else
		return false
	end
end

function CEnergyBeamEmitter:Ready()
	--self.m_AudioSource:PlayEvent("EnergyBeamEmitterReady")
	--iluminar
	self.m_Ready = false
end

function CEnergyBeamEmitter:Activatable()
	if self.m_Ready == true
		--self.m_AudioSource:PlayEvent("EnergyBeamEmitterActivatable")
		--Incrementar luminosidad
	end
end


