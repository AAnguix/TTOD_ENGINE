#ifndef _MATERIAL_H
#define _MATERIAL_H

#include "Utils\Named.h"
#include <vector>
#include "Math\Vector4.h"

class CRenderableObjectTechnique;
class CTexture;
class CXMLTreeNode;
class CMaterialParameter;

class CMaterial : public CNamed 
{ 
private:  
	std::vector<CTexture*>  m_Textures; 
	std::vector<CMaterialParameter*> m_Parameters;
	
	CRenderableObjectTechnique* m_RenderableObjectTechnique;
	unsigned int m_CurrentParameterData;
	unsigned int m_Offset;

	float m_StaticFriction;
	float m_DynamicFriction;
	float m_Restitution;

	void Destroy(); 

public:  
	CMaterial(CXMLTreeNode &TreeNode);  
	CMaterial(const CMaterial& Other);
	virtual ~CMaterial();  
	virtual void Apply(CRenderableObjectTechnique* RenderableObjectTechnique=NULL);  
	CRenderableObjectTechnique* GetRenderableObjectTechnique() const { return m_RenderableObjectTechnique; }

	void * GetNextParameterAddress(unsigned int NumBytes);
	const std::vector<CTexture*>& GetTextures() const { return m_Textures; }
	const std::vector<CMaterialParameter*>& GetParameters() const { return m_Parameters; }
	
	void SetFloatParameterValue(const std::string &ParameterName, float Value);
	void SetVect2fParameterValue(const std::string &ParameterName, Vect2f Value);
	void SetVect3fParameterValue(const std::string &ParameterName, Vect3f Value);
	void SetVect4fParameterValue(const std::string &ParameterName, Vect4f Value);

	void AddTexture(const std::string TextureFileName, bool GuiTexture);
	void ChangeTexture(const std::string Texture, size_t Index);

	float GetStaticFriction() const{ return m_StaticFriction; };
	float GetDynamicFriction() const{ return m_DynamicFriction; };
	float GetRestitution() const{ return m_Restitution; };

	CEmptyPointerClass* GetThisLuaAddress() const;
	CEmptyPointerClass* GetStaticFrictionLuaAddress() const;
	CEmptyPointerClass* GetDynamicFrictionLuaAddress() const;
	CEmptyPointerClass* GetRestitutionLuaAddress() const;
	void ApplyLuaPhysxProperties();
};

#endif
