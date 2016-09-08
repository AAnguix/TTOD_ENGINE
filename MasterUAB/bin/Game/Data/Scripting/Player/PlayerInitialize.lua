function CPlayerComponent:Initialize()

	-- local l_GameObject = self.m_LuaGameObject:GetGameObject()
	local l_CColliderName = self.m_LuaGameObject:GetName().."_CharacterCollider"
	
	g_PhysXManager:AddCharacterColliderComponent(l_CColliderName, self.m_LuaGameObject, self.m_Height, self.m_Radius, self.m_Density)
	
	local l_AudioSourceName = self.m_LuaGameObject:GetName().."_AudioSource"
	g_SoundManager:AddComponent(l_AudioSourceName, self.m_LuaGameObject)
	self.m_LuaGameObject:AddSound("SonidoDePrueba","Play_Hit")
	--self.m_AudioSource:AddSound("OpenMapSound","Play_Open_Map")
	
	--Animations
	local l_ACName = self.m_LuaGameObject:GetName().."_AnimatorController"
	g_AnimatorControllerManager:AddComponent(l_ACName, self.m_LuaGameObject)
	
	local l_Idle = self.m_LuaGameObject:AddState("Idle_State", "idle", 1.0, "OnEnter_Idle_Player", "OnUpdate_Idle_Player", "OnExit_Idle_Player")
	local l_Walk = self.m_LuaGameObject:AddState("Walk_State", "walk", 1.0, "OnEnter_Walk_Player", "OnUpdate_Walk_Player", "OnExit_Walk_Player")
	
	local l_AttacksAnimations = stringVector()
	local l_AnimationOne = "normalAttack"
	local l_AnimationTwo = "combatIdle"
	local l_AnimationThree = "die"
	l_AttacksAnimations:push_back(l_AnimationOne)
	l_AttacksAnimations:push_back(l_AnimationTwo)
	l_AttacksAnimations:push_back(l_AnimationThree)
	local l_RestartAnimationsTime = 1.5
	
	local l_Attack = self.m_LuaGameObject:AddState("Attack_State", "normalAttack", 1.0, "OnEnter_Attack_Player", "OnUpdate_Attack_Player", "OnExit_Attack_Player")
	local l_Block = self.m_LuaGameObject:AddState("Block_State", "block", 1.0, "OnEnter_Block_Player", "OnUpdate_Block_Player", "OnExit_Block_Player")
	local l_Injured = self.m_LuaGameObject:AddState("Injured_State", "die", 1.0, "OnEnter_Injured_Player", "OnUpdate_Injured_Player", "OnExit_Injured_Player")
	-- local l_Dead = self.m_LuaGameObject:AddState("Dead_State", "die", 1.0, "OnEnter_Dead_Player", "OnUpdate_Dead_Player", "OnExit_Dead_Player")
	local l_Tossed = self.m_LuaGameObject:AddState("Tossed_State", "combatIdle", 1.0, "OnEnter_Tossed_Player", "OnUpdate_Tossed_Player", "OnExit_Tossed_Player")
	local l_Interact = self.m_LuaGameObject:AddState("Interact_State", "interact", 1.0, "OnEnter_Interact_Player", "OnUpdate_Interact_Player", "OnExit_Interact_Player")
	local l_CombatIdle = self.m_LuaGameObject:AddState("CombatIdle_State", "combatIdle", 1.0, "OnEnter_CombatIdle_Player", "OnUpdate_CombatIdle_Player", "OnExit_CombatIdle_Player")
	
	self.m_LuaGameObject:AddBool("Walk", false)
	self.m_LuaGameObject:AddBool("SurroundingEnemies", false)
	self.m_LuaGameObject:AddTrigger("Attack", false)
	self.m_LuaGameObject:AddTrigger("Block", false)
	self.m_LuaGameObject:AddTrigger("TossedByDragon", false)
	self.m_LuaGameObject:AddTrigger("Interact", false)
	
	local l_IdleToWalk = l_Idle:AddTransition("IdleToWalk", l_Walk, false, 0.1)
	l_IdleToWalk:AddBoolCondition("Walk", true)
	
	local l_IdleToBlock = l_Idle:AddTransition("IdleToBlock", l_Block, false, 0.1, 0.2)
	l_IdleToBlock:AddTriggerCondition("Block")
	
	local l_IdleToTossed = l_Idle:AddTransition("IdleToTossed", l_Tossed, false, 0.2)
	l_IdleToTossed:AddTriggerCondition("TossedByDragon")
	
	local l_IdleToAttack = l_Idle:AddTransition("IdleToAttack", l_Attack, false, 0.2, 0.5)
	l_IdleToAttack:AddTriggerCondition("Attack")
	
	local l_IdleToCombatIdle = l_Idle:AddTransition("IdleToCombatIdle", l_CombatIdle, false, 0.2)
	l_IdleToCombatIdle:AddBoolCondition("SurroundingEnemies", true)
	
	local l_IdleToInteract = l_Idle:AddTransition("IdleToInteract", l_Interact, false, 0.1, 0.2)
	l_IdleToInteract:AddTriggerCondition("Interact")
	
	local l_WalkToIdle = l_Walk:AddTransition("WalkToIdle", l_Idle, false, 0.1)
	l_WalkToIdle:AddBoolCondition("Walk", false)
	l_WalkToIdle:AddBoolCondition("SurroundingEnemies", false)
	
	local l_WalkToCombatIdle = l_Walk:AddTransition("WalkToCombatIdle", l_CombatIdle, false, 0.1)
	l_WalkToCombatIdle:AddBoolCondition("Walk", false)
	l_WalkToCombatIdle:AddBoolCondition("SurroundingEnemies", true)
	
	local l_WalkToAttack = l_Walk:AddTransition("l_WalkToAttack", l_Attack, false, 0.1, 0.1)
	l_WalkToAttack:AddTriggerCondition("Attack")
	
	local l_WalkToTossed = l_Walk:AddTransition("WalkToTossed", l_Tossed, false, 0.2)
	l_WalkToTossed:AddTriggerCondition("TossedByDragon")
	
	local l_WalkToInteract = l_Walk:AddTransition("WalkToInteract", l_Interact, false, 0.1, 0.2)
	l_WalkToInteract:AddTriggerCondition("Interact")
	
	local l_AttackToIdle = l_Attack:AddTransition("AttackToIdle", l_Idle, true, 0.2)
	l_AttackToIdle:AddBoolCondition("SurroundingEnemies", false)
	
	local l_AttackToCombatIdle = l_Attack:AddTransition("AttackToCombatIdle", l_CombatIdle, true, 0.2)
	l_AttackToIdle:AddBoolCondition("SurroundingEnemies", true)
	
	local l_BlockToIdle = l_Block:AddTransition("BlockToIdle", l_Idle, true, 0.1)
	l_BlockToIdle:AddBoolCondition("SurroundingEnemies", false)
	
	local l_BlockToCombatIdle = l_Block:AddTransition("BlockToCombatIdle", l_CombatIdle, true, 0.1)
	l_BlockToCombatIdle:AddBoolCondition("SurroundingEnemies", true)
	
	local l_TossedToIdle = l_Tossed:AddTransition("TossedToIdle", l_Idle, true, 0.2)
	l_TossedToIdle:AddBoolCondition("SurroundingEnemies", false)
	
	local l_TossedToCombatIdle = l_Tossed:AddTransition("TossedToCombatIdle", l_CombatIdle, true, 0.2)
	l_TossedToIdle:AddBoolCondition("SurroundingEnemies", true)
	
	local l_CombatIdleToInteract = l_CombatIdle:AddTransition("CombatIdleToInteract", l_Interact, false, 0.1, 0.2)
	l_CombatIdleToInteract:AddTriggerCondition("Interact")
	
	local l_CombatIdleToWalk = l_CombatIdle:AddTransition("CombatIdleToWalk", l_Walk, false, 0.1)
	l_CombatIdleToWalk:AddBoolCondition("Walk", true)
	
	local l_CombatIdleToBlock = l_CombatIdle:AddTransition("CombatIdleToBlock", l_Block, false, 0.1, 0.2)
	l_CombatIdleToBlock:AddTriggerCondition("Block")
	
	local l_CombatIdleToTossed = l_CombatIdle:AddTransition("CombatIdleToTossed", l_Tossed, false, 0.2)
	l_CombatIdleToTossed:AddTriggerCondition("TossedByDragon")
	
	local l_CombatIdleToIdle = l_CombatIdle:AddTransition("CombatIdleToIdle", l_Idle, false, 0.2)
	l_CombatIdleToIdle:AddBoolCondition("SurroundingEnemies", false)
	
	local l_CombatIdleToAttack = l_CombatIdle:AddTransition("CombatIdleToAttack", l_Attack, false, 0.2, 0.5)
	l_CombatIdleToAttack:AddTriggerCondition("Attack")
	--ADD Inteact State

	self:InitializePlayerStats()
end