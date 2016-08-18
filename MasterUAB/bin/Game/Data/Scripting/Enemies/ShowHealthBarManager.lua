class 'CShowHealthBarManagerComponent' (CLUAComponent)
function CShowHealthBarManagerComponent:__init()
CLUAComponent.__init(self,"ShowHealthBarManager")	
	self.m_LastEnemyHit = nil
	self.m_X = 0.09
	self.m_Y = 0.05
	self.m_Width = 0.25
	self.m_Height = 0.05
	self.m_HealthBarID = "player_health_bar_0"
	self.m_HealthBarName = "player_health_bar"
	self.m_TimeShowingBar = 3.5
	g_EventManager:Subscribe(self, "ENEMY_TAKES_DAMAGE")
	self.m_Show = false
	self.m_LuaGameObject = nil
end

function CShowHealthBarManagerComponent:GetLuaGameObject() return self.m_LuaGameObject end

function CShowHealthBarManagerComponent:SetLastEnemyHit(Enemy)
	self.m_LastEnemyHit = Enemy
end

function CShowHealthBarManagerComponent:ENEMY_TAKES_DAMAGE()
	self.m_Show = true
	self:ResetTimer()
	--Coger el enemigo que ha recibido el golpe
end

function CShowHealthBarManagerComponent:Update(ElapsedTime)
	if self.m_Show then
		if self:GetTimer() <= self.m_TimeShowingBar then
			self:AddTime(ElapsedTime)
			self:ShowBar()
		else
			self:ResetTimer()
			self.m_Show = false
		end
	end
end

function CShowHealthBarManagerComponent:ShowBar()
	local l_EnemyCurrentHealth = self.m_LastEnemyHit:GetHealth() 
	local l_EnemyMaxHealth = self.m_LastEnemyHit:GetMaxHealth()
	local l_Avatar = self.m_LastEnemyHit:GetAvatar()
	if(l_Avatar~=nil and l_EnemyCurrentHealth>0.0) then 
		local l_HealthBarPos = SGUIPosition(self.m_X,self.m_Y,self.m_Width,self.m_Height,CGUIManager.TOP_LEFT,CGUIManager.GUI_RELATIVE,CGUIManager.GUI_RELATIVE_WIDTH)
		g_GUIManager:DoHealthBar(self.m_HealthBarID,self.m_HealthBarName,l_HealthBarPos, 0.0, l_EnemyMaxHealth, l_EnemyCurrentHealth) 	
		
		local l_AvatarPos = SGUIPosition(self.m_X-0.06, self.m_Y, 0.05, 0.05, CGUIManager.TOP_LEFT, CGUIManager.GUI_RELATIVE, CGUIManager.GUI_RELATIVE_WIDTH)
		g_GUIManager:DoImage("enemy_avatar", l_Avatar, l_AvatarPos)
	end
end

