-- This class calculates the damage for a
-- weapon/armor pair

class 'CDamageCalculator' 
function CDamageCalculator:__init()
	self.m_DamageReductionPct = {}
	self.m_Filename  = ""
	self.m_Weapons = {}
	self.m_Armors = {}
end

function CDamageCalculator:Destroy()
	for i=1, (#self.m_Weapons) do self.m_Weapons[i] = nil end
	for i=1, (#self.m_Armors) do self.m_Armors[i] = nil end
	for i=1, (#self.m_DamageReductionPct) do self.m_DamageReductionPct[i] = nil end
end

function CDamageCalculator:LogArmors()
	g_LogManager:Log("Armors:")
	for i=1, (#self.m_Armors) do g_LogManager:Log(self.m_Armors[i]) end
end
function CDamageCalculator:LogWeapons()
	g_LogManager:Log("Weapons:")	
	for i=1, (#self.m_Weapons) do g_LogManager:Log(self.m_Weapons[i]) end
end

function CDamageCalculator:CalculateDamage(ArmorType,WeaponType, Damage)
	local l_DamageReductionPercentage = self:GetPercentage(ArmorType,WeaponType)
	local l_Damage = Damage - ((Damage*l_DamageReductionPercentage)/100)
	return l_Damage
end

function CDamageCalculator:LoadXML(Filename)
	self.m_Filename = Filename
	local l_XMLTreeNode=CXMLTreeNode()
	local l_Loaded=l_XMLTreeNode:LoadFile(Filename)
	if l_Loaded then
		for i=0, l_XMLTreeNode:GetNumChildren() do
			local l_Element=l_XMLTreeNode:GetChild(i)
			local l_ElemName=l_Element:GetName()
			
			if l_ElemName=="damage_reduction_pair" then
				local l_ArmorType = l_Element:GetPszProperty("armor_type", "", false)
				local l_WeaponType = l_Element:GetPszProperty("weapon_type", "", false)
				local l_Percentage = l_Element:GetFloatProperty("percentage", 0.0, false)
				self:AddValue(l_ArmorType,l_WeaponType,l_Percentage)
			end
		end
		g_LogManager:Log("Weapons/Armors/DamageReductionPcts loaded.")
	else
		g_LogManager:Log("File '"..Filename.."'not correctly loaded")
	end
	--g_LogManager:Log("Game entities loaded...")
end 

function CDamageCalculator:AddValue(ArmorType,WeaponType,Percentage)
	if self:PairExists() == false then
		
		if (self:ArmorExists(ArmorType)==false) then
			self:AddArmor(ArmorType)
		end
		if (self:WeaponExists(WeaponType)==false) then
			self:AddWeapon(WeaponType)
			g_LogManager:Log("Weapon "..WeaponType.." created")
		end
	
		local l_DamageReductionPair = CDamageReductionPair(ArmorType,WeaponType,Percentage)
		local l_Key = ""..ArmorType.."_"..WeaponType
		self.m_DamageReductionPct[l_Key] = l_DamageReductionPair
	end
end

function CDamageCalculator:ArmorExists(ArmorType)
	for i=1, (#self.m_Armors) do 
		if (self.m_Armors[i]==ArmorType) then
			return true
		end
	end	
	return false
end
function CDamageCalculator:WeaponExists(WeaponType)
	for i=1, (#self.m_Weapons) do 
		if (self.m_Weapons[i]==WeaponType) then
			--g_LogManager:Log("Arma.."..WeaponType.." existe")
			return true
		end
	end	
	--g_LogManager:Log("Arma.."..WeaponType.." no existe")
	return false
end

function CDamageCalculator:AddArmor(ArmorType)
	local l_Size = #self.m_Armors+1
	self.m_Armors[l_Size] = ArmorType
end
function CDamageCalculator:AddWeapon(WeaponType)
	local l_Size = #self.m_Weapons+1
	self.m_Weapons[l_Size] = WeaponType
end

function CDamageCalculator:GetPercentage(Armor,Weapon)
	local l_Key = ""..Armor.."_"..Weapon
	return self.m_DamageReductionPct[l_Key].m_Percentage
end

function CDamageCalculator:PairExists(Armor, Weapon)
	for i=1, (#self.m_DamageReductionPct) do
		if self.m_DamageReductionPct[i].m_Armor == Armor and self.m_DamageReductionPct[i].m_Weapon == Weapon then
			return true
		end
	end
	return false
end

class 'CDamageReductionPair'
function CDamageReductionPair:__init(Armor,Weapon,Percentage)
	self.m_Armor = Armor
	self.m_Weapon = Weapon
	self.m_Percentage = Percentage
end
