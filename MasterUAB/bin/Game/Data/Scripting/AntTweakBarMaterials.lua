function OpenMaterials()
	ClickOnElement("Materials")
	g_DebugHelper:RegisterButton("Export[XML]","WriteMaterialsInfoToXml()")
	local l_Materials=CEngine.GetSingleton():GetMaterialManager():GetLUAMaterials()
	for l_Material in l_Materials do
		g_DebugHelper:RegisterButton(l_Material:GetName(),"OpenMaterial('"..l_Material:GetName().."')")
	end
end

function OpenMaterial(MaterialName)
	ClickOnElement(MaterialName)
	local l_Material = CEngine.GetSingleton():GetMaterialManager():GetResource(MaterialName)
	local l_Parameters = l_Material:GetParameters()
	for l_Param in l_Parameters do
		if l_Param:GetMaterialType() == CMaterialParameter.float then
			g_DebugHelper:RegisterFloatParameter(l_Param:GetName(),l_Param:GetValueLuaAddress(),l_Param:GetDescription())
		elseif l_Param:GetMaterialType() == CMaterialParameter.vect2f then 
			g_DebugHelper:RegisterVect2fParameter(l_Param:GetName(),l_Param:GetValueLuaAddress(),l_Param:GetDescription())
		elseif l_Param:GetMaterialType() == CMaterialParameter.vect3f then 
			g_DebugHelper:RegisterVect3fParameter(l_Param:GetName(),l_Param:GetValueLuaAddress(),l_Param:GetDescription())
		elseif l_Param:GetMaterialType() == CMaterialParameter.vect4f then 
			g_DebugHelper:RegisterVect4fParameter(l_Param:GetName(),l_Param:GetValueLuaAddress(),l_Param:GetDescription())
		end
	end
	
	g_DebugHelper:RegisterFloatParameter("static_friction",l_Material:GetStaticFrictionLuaAddress(),"")
	g_DebugHelper:RegisterFloatParameter("dynamic_friction",l_Material:GetDynamicFrictionLuaAddress(),"")
	g_DebugHelper:RegisterFloatParameter("restitution",l_Material:GetRestitutionLuaAddress(),"")
	g_DebugHelper:RegisterExtendedButton("Apply Physx.","ApplyPhysxProperties",l_Material:GetThisLuaAddress(),"material")
	
	local l_Textures = l_Material:GetTextures()
	local l_Index = 0
	for l_Texture in l_Textures do
		g_DebugHelper:RegisterStringParameter("Texture",l_Texture:GetNameLuaAddress(),"")
		--l_DebugHelper:RegisterButtonV2("Reload ","ReloadTexture",l_Texture:GetNameLuaAddress())
		g_DebugHelper:RegisterChangeTextureButton("Reload texture ","ChangeTexture",l_Material,l_Texture:GetNameLuaAddress(),l_Index)
		l_Index = l_Index+1
	end
end

function ApplyPhysxProperties(Material)
	Material:ApplyLuaPhysxProperties()
end

function ChangeTexture(Material, TextureName, Index)
	local l_NewTexturePath = CTTODFileUtils.SelectTextureFile(nil, TextureName)
	if l_NewTexturePath ~= "" then
		Material:ChangeTexture(l_NewTexturePath, Index)
	end
end

function WriteMaterialsInfoToXml()

	local Filename = "Data/Level"..g_CurrentLevel.."/materials_WRITTING.xml"
	local l_Writer = CTTODXMLWriter()
	l_Writer:StartFile(Filename)
		local l_Materials=CEngine.GetSingleton():GetMaterialManager():GetLUAMaterials()
		for l_Material in l_Materials do
			l_Writer:StartElement("material", true)
				l_Writer:WriteStringProperty("name", l_Material:GetName())
				l_Writer:WriteStringProperty("renderable_object_technique", l_Material:GetRenderableObjectTechnique():GetName())
				--g_LogManager:Log("Material escrito "..l_Material:GetName())
				
				WriteMaterialTextures(l_Writer, l_Material)
				WriteMaterialParameters(l_Writer, l_Material)
			l_Writer:EndElement()	
		end
	l_Writer:EndFile()
end

function WriteMaterialTextures(Writer, Material)
	local l_Textures = Material:GetTextures()
	for l_Texture in l_Textures do
		Writer:StartElement("texture", true)
			Writer:WriteStringProperty("filename", l_Texture:GetName())
			--g_LogManager:Log("textura escrita")
		Writer:EndElement()	
	end
end

function WriteMaterialParameters(Writer, Material)
	local l_Parameters = Material:GetParameters()
	for l_Param in l_Parameters do
		Writer:StartElement("parameter", true)
			if l_Param:GetMaterialType() == CMaterialParameter.float then
				Writer:WriteStringProperty("type","float")
				Writer:WriteStringProperty("name",l_Param:GetName())
				Writer:WriteFloatProperty("value",l_Param:GetValue())
			elseif l_Param:GetMaterialType() == CMaterialParameter.vect2f then 
				Writer:WriteStringProperty("type","vect2f")
				Writer:WriteStringProperty("name",l_Param:GetName())
				Writer:WriteVect2fProperty(l_Param:GetName(), l_Param:GetValue())
			elseif l_Param:GetMaterialType() == CMaterialParameter.vect3f then 
				Writer:WriteStringProperty("type","vect3f")
				Writer:WriteStringProperty("name",l_Param:GetName())
				Writer:WriteVect3fProperty(l_Param:GetName(), l_Param:GetValue())
			elseif l_Param:GetMaterialType() == CMaterialParameter.vect4f then 
				Writer:WriteStringProperty("type","vect4f")
				Writer:WriteStringProperty("name",l_Param:GetName())
				Writer:WriteVect4fProperty(l_Param:GetName(), l_Param:GetValue())
			end
			--g_LogManager:Log("parametro escrito")
			Writer:WriteStringProperty("description",l_Param:GetDescription())
		Writer:EndElement()
	end	
end