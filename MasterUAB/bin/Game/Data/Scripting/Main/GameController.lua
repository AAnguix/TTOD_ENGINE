dofile("./Data/Scripting/Weapons/DamageCalculator.lua")

class 'CGameController'

function CGameController:__init()
	self.m_Entities={}
	self.m_Filename = ""
end

function CGameController:Update(ElapsedTime)
	for i=1, (#self.m_Entities) do
		self.m_Entities[i]:Update(ElapsedTime)
	end
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
	g_LayerManager:RemoveLuaComponents()
	g_LayerManager:RemoveComponent("AnimatorController") --removes this component from all layers
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
		
		l_RObject:AddLuaComponent(l_EnemyComponent)
		l_EnemyComponent:Initialize()
		CGameController:LoadEnemyChilds(l_EnemyComponent,XMLTreeNode)
		table.insert(self.m_Entities,l_EnemyComponent)
	end
end

function CGameController:LoadWeapon(TreeNode, EnemyComponent)
	local l_WeaponRObject = GetElementFromLayer(XMLTreeNode)
	if l_WeaponRObject ~= nil then
		
		local l_Parent = EnemyComponent.m_RObject
		local l_BoneName = TreeNode:GetPszProperty("owner_bone_name", "", false)
		local l_WeaponType = TreeNode:GetPszProperty("type", "", false)
		local l_Damage = TreeNode:GetFloatProperty("damage", "", false)
		
		local l_WeaponComponent  = CWeaponComponent("weapon",l_Parent, l_WeaponRObject, l_Damage, l_WeaponType)
		l_WeaponRObject:AddLuaComponent(l_WeaponComponent)
		l_WeaponComponent:Initialize(l_BoneName)
		
		table.insert(self.m_Entities,l_WeaponComponent)
	end
end

function CGameController:LoadArmor(TreeNode, EnemyComponent)
	local l_ArmorRObject = GetElementFromLayer(XMLTreeNode)
	if l_ArmorRObject ~= nil then
	
		local l_Parent = EnemyComponent.m_RObject
		local l_ArmorType = TreeNode:GetPszProperty("type", "", false)
		local l_Damage = TreeNode:GetFloatProperty("resistance", "", false)
	
		local l_ArmorComponent  = CArmorComponent("armor",l_Parent, l_ArmorRObject, l_Damage, l_ArmorType)
		l_ArmorRObject:AddLuaComponent(l_ArmorComponent)
		l_ArmorComponent:Initialize()
		
		table.insert(self.m_Entities,l_ArmorComponent)
	end
end

function CGameController:LoadPlayer(XMLTreeNode)
	
	local l_RObject = GetElementFromLayer(XMLTreeNode)
	g_LayerManager:SetPlayer(l_RObject)
	
	local l_PlayerComponent=CPlayerComponent()
	l_RObject:AddLuaComponent(l_PlayerComponent)
	
	l_PlayerComponent:Initialize(l_RObject)
	g_PlayerComponent = l_PlayerComponent

	table.insert(self.m_Entities,l_PlayerComponent)
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

function CGameController:LoadEnemyChilds(Enemy, XMLTreeNode)
	for i=0, XMLTreeNode:GetNumChildren() do
		local l_Element=XMLTreeNode:GetChild(i)
		if l_Element:GetName()=="way_point" then
			local l_V = Vect3f(0.0,0.0,0.0)
			l_V = l_Element:GetVect3fProperty("way_point",Vect3f(0.0,0.0,0.0),false)
			Enemy:AddWaypoint(l_V)
		elseif l_Element:GetName()=="weapon" then
			LoadWeapon(l_Element,Enemy)
		elseif l_Element:GetName()=="armor" then
			LoadArmor(l_Element,Enemy)
		end
	end
end	

function GetElementFromLayer(XMLTreeNode)
	local l_RenderableObject = XMLTreeNode:GetPszProperty("renderable_object", "", false)
	local l_LayerName = XMLTreeNode:GetPszProperty("layer", "", false)
	local l_RObject = g_LayerManager:GetResource(l_LayerName):GetResource(l_RenderableObject)
	return l_RObject
end