#include "RenderableObject.h"
#include <assert.h>
#include "XML\XMLTreeNode.h"
#include "Utils\GameObjectManager.h"
#include "Engine\Engine.h"

CRenderableObject::CRenderableObject(const CXMLTreeNode &XMLTreeNode) 
:CComponent(XMLTreeNode.GetPszProperty("name"), CEngine::GetSingleton().GetGameObjectManager()->GetResource(XMLTreeNode.GetPszProperty("name")), XMLTreeNode.GetBoolProperty("visible", true))
,C3DElement(XMLTreeNode)
{
	
}

CRenderableObject::CRenderableObject(CGameObject *Owner, const std::string &Name, const Vect3f &Position, float Yaw, float Pitch, float Roll)
:CComponent(Name, Owner)
,C3DElement(Position,Yaw,Pitch,Roll)
{

}

CRenderableObject::~CRenderableObject()
{

}

void CRenderableObject::Update(float ElapsedTime)
{
	
}

CEmptyPointerClass* CRenderableObject::GetThisLuaAddress() const { return (CEmptyPointerClass *)(this); }