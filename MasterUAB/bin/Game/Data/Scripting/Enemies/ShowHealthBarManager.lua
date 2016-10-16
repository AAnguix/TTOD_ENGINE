class 'CShowHealthBarManagerComponent' (CLUAComponent)
function CShowHealthBarManagerComponent:__init(CLuaGameObject)
CLUAComponent.__init(self,"ShowHealthBarManager")	
	self.m_LastEnemyHit = nil

	self.m_HBBackgroundX = 0.45
	self.m_HBX = self.m_HBBackgroundX + 0.1
	self.m_AvatarX = self.m_HBBackgroundX - 0.04
	
	self.m_HBBackgroundY = 0.07
	self.m_HBY = self.m_HBBackgroundY + 0.01
	self.m_AvatarY = self.m_HBBackgroundY - 0.034
	
	self.m_HBBackgroundWidth = 0.2
	self.m_HBWidth = self.m_HBBackgroundWidth*0.8
	
	self.m_HBBackgrounHeight = 0.025
	self.m_HBHeight = self.m_HBBackgrounHeight/1.65 --0.015
	
	self.m_AvatarWidth = 0.07
	self.m_AvatarHeight = self.m_AvatarWidth
	
	self.m_HealthBarID = "enemy_health_bar"
	self.m_HealthBarName = "player_health_bar"
	self.m_TimeShowingBar = 5.0
	g_EventManager:Subscribe(self, "ENEMY_TAKES_DAMAGE")
	self.m_Show = false
	self.m_LuaGameObject = CLuaGameObject
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
		local l_HealthBarBackgroundPos = SGUIPosition(self.m_HBBackgroundX,self.m_HBBackgroundY,self.m_HBBackgroundWidth,self.m_HBBackgrounHeight,CGUIManager.TOP_LEFT, CGUIManager.GUI_RELATIVE, CGUIManager.GUI_RELATIVE_WIDTH)
		g_GUIManager:DoImage("healthbar_image_background", "healthbar_image_background", l_HealthBarBackgroundPos)
		local l_HealthBarPos = SGUIPosition(self.m_HBX,self.m_HBY,self.m_HBWidth,self.m_HBHeight,CGUIManager.TOP_CENTER,CGUIManager.GUI_RELATIVE,CGUIManager.GUI_RELATIVE_WIDTH)
		g_GUIManager:DoHealthBar(self.m_HealthBarID,self.m_HealthBarName,l_HealthBarPos, 0.0, l_EnemyMaxHealth, l_EnemyCurrentHealth) 	
		
		local l_AvatarPos = SGUIPosition(self.m_AvatarX, self.m_AvatarY, self.m_AvatarWidth,self.m_AvatarHeight, CGUIManager.TOP_LEFT, CGUIManager.GUI_RELATIVE, CGUIManager.GUI_RELATIVE_WIDTH)
		g_GUIManager:DoImage("enemy_avatar", l_Avatar, l_AvatarPos)
	end
end

