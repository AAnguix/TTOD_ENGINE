#include "Cinematics\CinematicObjectKeyFrame.h"
#include "XML\XMLTreeNode.h"

CCinematicObjectKeyFrame::CCinematicObjectKeyFrame(CXMLTreeNode &XMLTreeNode)
{
	m_KeyFrameTime=XMLTreeNode.GetFloatProperty("time",0.0f);
	m_Position=XMLTreeNode.GetVect3fProperty("pos",v3fZERO);
	m_Yaw=XMLTreeNode.GetFloatProperty("yaw",0.0f);
	m_Pitch=XMLTreeNode.GetFloatProperty("pitch",0.0f);
	m_Roll=XMLTreeNode.GetFloatProperty("roll",0.0f);
	m_Scale=XMLTreeNode.GetFloatProperty("scale",0.0f);
}

float CCinematicObjectKeyFrame::GetKeyFrameTime() const
{
	return m_KeyFrameTime;
}

void CCinematicObjectKeyFrame::SetKeyFrameTime(const float KeyFrameTime)
{
	m_KeyFrameTime=KeyFrameTime;
}