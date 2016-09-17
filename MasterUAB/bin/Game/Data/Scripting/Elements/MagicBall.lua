class 'CMagicBallComponent' (CLUAComponent)
function CMagicBallComponent:__init(CLuaGameObject)
CLUAComponent.__init(self,"MagicBallScript")	
	self.m_LuaGameObject = CLuaGameObject
	self.m_Damage = 50.0
	self.m_StunTime = 2.0
	self.m_MaxScale = 1.0
	self.m_Speed = 4.0
	
	g_EventManager:Subscribe( self, "START_GROWING_MAGIC_BALL" )
	g_EventManager:Subscribe( self, "STOP_GROWING_MAGIC_BALL" )
	g_EventManager:Subscribe( self, "THROW_MAGIC_BALL" )
	g_EventManager:Subscribe( self, "DRAGON_IMPACTED_BY_MAGIC_BALL" )
	self.m_Destiny = Vect3f(0.0,0.0,0.0)
	self.m_Growing = false
	self.m_Thrown = false
	
	g_PhysXManager:CreateSphereTrigger(self.m_LuaGameObject:GetName(),1.5,"",Vect3f(0.0,0.0,0.0),Quatf(0.0,0.0,0.0,1.0),"kinematic")
	g_PhysXManager:ChangeShapeTriggerState(self.m_LuaGameObject:GetName(),false, false)
end

function CMagicBallComponent:START_GROWING_MAGIC_BALL(MagicBallPosition)
	--self.m_Thrown = false
	self.m_Growing = true
	self.m_LuaGameObject:SetPosition(MagicBallPosition)
	self.m_LuaGameObject:EnableRenderableObject(true)
	g_PhysXManager:MoveKinematicActor(self.m_LuaGameObject:GetName(), MagicBallPosition, Quatf(0.0,0.0,0.0,1.0))
	g_LogManager:Log("bola comienza a crecer")
end

function CMagicBallComponent:STOP_GROWING_MAGIC_BALL()
	g_LogManager:Log("bola para de crecer")
	self.m_Growing = false
end

function CMagicBallComponent:THROW_MAGIC_BALL()
	g_LogManager:Log("se lanza la bola")
	self.m_Thrown = true
	--self.m_Growing = false
	self.m_Destiny = g_PlayerComponent:GetLuaGameObject():GetPosition()
	g_PhysXManager:ChangeShapeTriggerState(self.m_LuaGameObject:GetName(),true, false)
end

function CMagicBallComponent:DRAGON_IMPACTED_BY_MAGIC_BALL()
	self.m_Thrown = false
	self.m_LuaGameObject:EnableRenderableObject(false)
	g_PhysXManager:ChangeShapeTriggerState(self.m_LuaGameObject:GetName(),false, false)
end

function CMagicBallComponent:OnTriggerEnter(Actor)
	if (Actor=="Player") then
		--TODO CAMBIAR POR EL DRAGON
		g_PlayerComponent:TakeSimpleDamage(self.m_Damage)
		g_EventManager:FireEvent("DRAGON_IMPACTED_BY_MAGIC_BALL")
	end
end

function CMagicBallComponent:GetLuaGameObject() return self.m_LuaGameObject end

function CMagicBallComponent:GetStunTime() return self.m_StunTime end
function CMagicBallComponent:GetDamage() return self.m_Damage end

function CMagicBallComponent:Update(ElapsedTime)
	if self.m_Growing then
		self:Growing(ElapsedTime)
	elseif self.m_Thrown then
		self:Throw(ElapsedTime)
	end
end

function CMagicBallComponent:Throw(ElapsedTime)
	local l_Position = self.m_LuaGameObject:GetPosition()
	local l_Displacement = self.m_Destiny - l_Position
	l_Displacement:Normalize(1)
	local l_Constant = 3.0
	local l_Base = 30.0 -- 1.1
	local l_Exponent = self:GetTimer() -- math.sqrt(self:GetTimer())
	local l_NewPosition = l_Position + ((ExponentialDisplacement(l_Displacement,l_Constant,l_Base,l_Exponent))*ElapsedTime*self.m_Speed)
	self.m_LuaGameObject:SetPosition(l_NewPosition)
	
	g_PhysXManager:MoveKinematicActor(self.m_LuaGameObject:GetName(), l_Position, Quatf(0.0,0.0,0.0,1.0))
end

function CMagicBallComponent:Growing(ElapsedTime)
	--Apply specific shader and grow
end