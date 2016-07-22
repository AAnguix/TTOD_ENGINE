class 'CMagicBall' (CActivableElement)
function CMagicBall:__init(CLuaGameObject)
	self.m_LuaGameObject = CLuaGameObject
	self.m_Damage = 50.0
	self.m_StunTime = 2.0
end

function CMagicBall:GetStunTime() return self.m_StunTime end
function CMagicBall:GetDamage() return self.m_Damage end

function CMagicBall:Update(ElapsedTime)
end

