#ifndef _TEMPLATEDMATERIALPARAMETER_H
#define _TEMPLATEDMATERIALPARAMETER_H

#include "Materials\MaterialParameter.h"
//#include "Effects\EffectManager.h"

template<typename T>
class CTemplatedMaterialParameter : public CMaterialParameter
{

private:
	T m_Value;
	void *m_EffectAddress;
	std::string m_Description;

public:
	CTemplatedMaterialParameter(CMaterial *Material, CXMLTreeNode &TreeNode, const T &Value, CMaterialParameter::TMaterialType MaterialType) : CMaterialParameter(Material,TreeNode,MaterialType)
	{
		//m_EffectAddress=CEffectManager::AddMaterialParameter(MaterialType);
		m_Value=Value;
		m_Description=TreeNode.GetPszProperty("description","");
		m_EffectAddress=Material->GetNextParameterAddress(sizeof(T));
	};
	CTemplatedMaterialParameter(CMaterial *Material, const std::string &Name, const std::string &Description, const T &Value, CMaterialParameter::TMaterialType MaterialType) : CMaterialParameter(Material, Name, MaterialType)
	{
		//m_EffectAddress=CEffectManager::AddMaterialParameter(MaterialType);
		m_Value = Value;
		m_Description = Description;
		m_EffectAddress = Material->GetNextParameterAddress(sizeof(T));
	};
	virtual ~CTemplatedMaterialParameter(){};
	void Apply()
	{ 
		memcpy(m_EffectAddress,&m_Value,sizeof(T)); 
	};
	void * GetValueAddress() const{return (void*)&m_Value;};
	T GetValue() const { return m_Value; };
	void SetValue(T Value) { m_Value = Value; }
	const std::string &GetDescription(){return m_Description;};
};

#endif