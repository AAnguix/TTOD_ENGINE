#ifndef _MATERIALPARAMETER_H
#define _MATERIALPARAMETER_H

#include "Utils\Named.h"
#include "Utils\Utils.h"
#include "Utils\EmptyPointerClass.h"

class CMaterial;

class CMaterialParameter : public CNamed
{

public:
	enum TMaterialType
	{
		FLOAT=0,
		VECT2F,
		VECT3F,
		VECT4F
	};

protected:
	TMaterialType m_MaterialType;

public:
	CMaterialParameter(CMaterial *Material, CXMLTreeNode &TreeNode, CMaterialParameter::TMaterialType MaterialType);
	CMaterialParameter(CMaterial *Material, const std::string &Name, CMaterialParameter::TMaterialType MaterialType);
	virtual ~CMaterialParameter();
	virtual void Apply() = 0;
	virtual void * GetValueAddress() const = 0;
	UAB_GET_STANDARD_PROPERTY(TMaterialType, MaterialType);
	virtual CEmptyPointerClass * GetValueLuaAddress() const {return (CEmptyPointerClass *)GetValueAddress();}
	virtual const std::string &GetDescription() = 0;
};

#endif