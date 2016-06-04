dofile("./Data/Scripting/Weapons/DamageCalculator.lua")

class 'CGameController'

g_Weapon = nil

function CGameController:__init()
	self.m_Entities={}
	self.m_Filename = ""
end

function CGameController:Update(ElapsedTime)
	-- for i=1, (#self.m_Entities) do
		-- self.m_Entities[i]:Update(ElapsedTime)
	-- end
end

function CGameController:LoadXML(Filename)
	self.m_Filename = Filename
	self.m_Entities={}
	local l_XMLTreeNode=CXMLTreeNode()
	local l_Loaded=l_XMLTreeNode:LoadFile(Filename)
	if l_Loaded and l_XMLTreeNode:GetName() == "game_entities" then
		for i=0, l_XMLTreeNode:GetNumChildren() do
			local l_Element=l_XMLTreeNode:GetChild(i)
			local l_ElemName=l_Element:GetName()
			
			if l_ElemName=="enemy" then
				self:LoadEnemy(l_Element)
			elseif l_ElemName=="player" then
				self:LoadPlayer(l_Element)
			elseif l_ElemName=="pedestal" then
				self:LoadPedestal(l_Element)
			elseif l_ElemName=="dragon" then
				self:LoadDragon(l_Element)
			end 
		end
		g_LogManager:Log("Game entities loaded.")
	else
		print("File '"..Filename.."'not correctly loaded")
	end
	--g_LogManager:Log("Game entities loaded...")
end 

function CGameController:Reload()
	for i=1, (#self.m_Entities) do
		self.m_Entities[i] = nil
	end
	--g_LayerManager:RemoveLuaComponents()
	--g_LayerManager:RemoveComponent("AnimatorController") --removes this component from all layers
	self:LoadXML(self.m_Filename)
end

function CGameController:LoadEnemy(XMLTreeNode)
	local l_RObject = GetElementFromLayer(XMLTreeNode)
	
	if l_RObject ~= nil then

		local l_EnemyType = XMLTreeNode:GetPszProperty("enemy_type", "basic_enemy", false)
		local l_EnemyComponent = nil
		
		if l_EnemyType == "basic_enemy" then
			l_EnemyComponent  = CBasicEnemyComponent(l_RObject)
		elseif l_EnemyType == "ranged_enemy" then
			l_EnemyComponent  = CRangedEnemyComponent(l_RObject)
		elseif l_EnemyType == "brute_enemy" then
			l_EnemyComponent  = CBruteEnemyComponent(l_RObject)
		end
		
		
		local l_ComponentName = l_RObject:GetName().."_Script"
		g_ScriptManager:AddComponent(l_ComponentName,l_RObject,l_EnemyComponent)
		
		l_EnemyComponent:Initialize()
		CGameController:LoadEnemyChilds(l_EnemyComponent,XMLTreeNode)
		table.insert(self.m_Entities,l_EnemyComponent)
	end
end

function CGameController:LoadEnemyChilds(Enemy, XMLTreeNode)
	for i=0, XMLTreeNode:GetNumChildren() do
		local l_Element=XMLTreeNode:GetChild(i)
		if l_Element:GetName()=="way_point" then
			local l_V = Vect3f(0.0,0.0,0.0)
			l_V = l_Element:GetVect3fProperty("way_point",Vect3f(0.0,0.0,0.0),false)
			Enemy:AddWaypoint(l_V)
		elseif l_Element:GetName()=="weapon" then
			self:LoadWeapon(l_Element,Enemy)
		elseif l_Element:GetName()=="armor" then
			self:LoadArmor(l_Element,Enemy)
		end
	end
end	

function CGameController:LoadWeapon(TreeNode, EnemyComponent)
	local l_WeaponRObject = GetElementFromLayer(TreeNode)
	if l_WeaponRObject ~= nil then
		
		local l_Parent = EnemyComponent.m_RObject
		local l_BoneName = TreeNode:GetPszProperty("parent_bone", "", false)
		local l_WeaponType = TreeNode:GetPszProperty("type", "", false)
		local l_Damage = TreeNode:GetFloatProperty("damage", 0.0, false)
													--ComponentType, ParentRObject, MeshRObject, Damage, WeaponType)
		local l_WeaponComponent = CWeaponComponent("weapon",l_Parent, l_BoneName, l_WeaponRObject, l_Damage, l_WeaponType)
		
		g_Weapon = l_WeaponComponent
		-- table.insert(self.m_Entities,l_WeaponComponent)
		local l_ComponentName = l_WeaponRObject:GetName().."_Script"
		g_ScriptManager:AddComponent(l_ComponentName,l_WeaponRObject,l_WeaponComponent)
		EnemyComponent:SetWeapon(l_ArmorComponent)
	
	end
end

function CGameController:LoadArmor(TreeNode, EnemyComponent)
	local l_ArmorRObject = GetElementFromLayer(XMLTreeNode)
	if l_ArmorRObject ~= nil then
	
		local l_Parent = EnemyComponent.m_RObject
		local l_BoneName = TreeNode:GetPszProperty("parent_bone", "", false)
		local l_ArmorType = TreeNode:GetPszProperty("type", "", false)
		local l_Damage = TreeNode:GetFloatProperty("resistance", 0.0, false)
	
		local l_ArmorComponent  = CArmorComponent("armor",l_Parent, l_BoneName, l_ArmorRObject, l_Damage, l_ArmorType)
		l_ArmorRObject:AddLuaComponent(l_ArmorComponent)
		EnemyComponent:SetArmor(l_ArmorComponent)
		-- l_ArmorComponent:Initialize(l_BoneName)
		
		table.insert(self.m_Entities,l_ArmorComponent)
	end
end

function CGameController:LoadPlayer(XMLTreeNode)
	
	local l_RObject = GetElementFromLayer(XMLTreeNode)
	g_LayerManager:SetPlayer(l_RObject) --Tells layer manager who is the player.
	g_Player = l_RObject --Tells Lua ...
	
	local l_PlayerComponent=CPlayerComponent(l_RObject)
	local l_ComponentName = l_RObject:GetName().."_Script"
	g_ScriptManager:AddComponent(l_ComponentName,l_RObject,l_PlayerComponent)
	
	l_PlayerComponent:Initialize()
	g_PlayerComponent = l_PlayerComponent

	table.insert(self.m_Entities,l_PlayerComponent)
end

function CGameController:LoadDragon(XMLTreeNode)
	
	local l_RObject = GetElementFromLayer(XMLTreeNode) 
	local l_ParticleEmitter = GetParticleEmitter(XMLTreeNode)
	local l_BoneName = XMLTreeNode:GetPszProperty("bone_name", "", false)
	local l_ModelName = XMLTreeNode:GetPszProperty("model_name", "", false)
	
	--local l_CoreModel = g_AnimatedModelManager:GetResource(l_ModelName)
	local l_CoreModel = l_RObject:GetAnimatedCoreModel()
	local l_BoneID = l_CoreModel:GetBoneId(l_BoneName)
	
	if l_RObject ~= nil and l_ParticleEmitter ~= nil and l_BoneName ~= "" then
		local l_DragonComponent = CDragonComponent(l_RObject,l_ParticleEmitter,l_BoneID)
		l_RObject:AddLuaComponent(l_DragonComponent)
		l_DragonComponent:Initialize()
		table.insert(self.m_Entities,l_DragonComponent)
	end
end

function CGameController:LoadPedestal(XMLTreeNode)
	
	--g_LogManager:Log(XMLTreeNode:GetPszProperty("gui_id", "", false))
	
	local l_RObject = GetElementFromLayer(XMLTreeNode)
	local l_PedestalComponent=CPedestalComponent(l_RObject)
	l_PedestalComponent:Initialize(XMLTreeNode)
	l_RObject:AddLuaComponent(l_PedestalComponent)
	
	l_PedestalComponent:Initialize(l_RObject)
	table.insert(self.m_Entities,l_PedestalComponent)
end

function GetElementFromLayer(XMLTreeNode)
	local l_RenderableObject = XMLTreeNode:GetPszProperty("renderable_object", "", false)
	local l_LayerName = XMLTreeNode:GetPszProperty("layer", "", false)
	local l_RObject = g_LayerManager:GetResource(l_LayerName):GetResource(l_RenderableObject)
	return l_RObject
end
function GetParticleEmitter(XMLTreeNode)
	local l_Particle = XMLTreeNode:GetPszProperty("particle_emiter", "", false)
	local l_LayerName = "particles"
	local l_RObject = g_LayerManager:GetResource(l_LayerName):GetResource(l_Particle)
	return l_RObject
end