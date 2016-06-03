#include "Cinematics\CameraKeyController.h"
#include "Cinematics\CameraKey.h"
#include "XML\XMLTreeNode.h"
#include "Utils\Utils.h"
#include "Camera\Camera.h"
#include "Engine.h"
#include "Log\Log.h"
#include "Camera\CameraControllerManager.h"

#include <sstream>
#include <iostream>

void CCameraKeyController::LoadXML(const std::string &FileName)
{
	CXMLTreeNode l_XML;

	if (l_XML.LoadFile(FileName.c_str()))
	{
		CXMLTreeNode l_CameraKeys = l_XML["camera_key_controller"];

		if (l_CameraKeys.Exists())
		{
			for (int i = 0; i < l_CameraKeys.GetNumChildren(); ++i)
			{
				CXMLTreeNode l_Element = l_CameraKeys(i);

				if (l_Element.GetName() == std::string("key"))
				{
					CCameraInfo l_CameraInfo(l_Element);
					float l_Time=l_Element.GetFloatProperty("time",0.0f);
					CCameraKey *l_CameraKey=new CCameraKey(l_CameraInfo,l_Time);
					m_TotalTime=l_Time;
					m_Keys.push_back(l_CameraKey);
				}
			}
		}
	}
}

CCameraKeyController::~CCameraKeyController()
{
	for(size_t i=0;i<m_Keys.size();++i)
	{
		CHECKED_DELETE(m_Keys[i]);
	}
	m_Keys.clear();
}

void CCameraKeyController::GetCurrentKey()
{
	if(IsCycle())
	{
		if(m_CurrentTime>m_TotalTime)
		{
			ResetTime();
			m_CurrentKey=0;
			m_NextKey=1;
		} 
		else
		{
			for(size_t i=m_CurrentKey;i<m_Keys.size()-1;++i)
			{
				if(m_Keys[i]->m_Time>m_CurrentTime)
				{
					m_CurrentKey=i;
					m_NextKey=i+1;
					i=m_Keys.size()-1;
				}
			}
		}
	}
	else if(IsReverse())
	{
		if(m_Backing)
		{
			if(m_CurrentTime<m_TotalTime)
			{
				for(size_t i=m_CurrentKey;i>0;--i)
				{
					//(m_Keys[i]->m_Time>=m_CurrentTime
					if(m_Keys[i]->m_Time <= (m_TotalTime-m_CurrentTime))
					{
						m_CurrentKey=i;
						m_NextKey=i-1;
					}
				}
			} 
			else
			{
				m_Backing=false;
				ResetTime();
				m_CurrentKey=0;
				m_NextKey=1;
			}
		}
		else
		{
			if(m_CurrentTime<m_TotalTime)
			{
				for(size_t i=0;i<m_Keys.size();++i)
				{
					if(m_CurrentTime>=m_Keys[i]->m_Time)
					{
						m_CurrentKey=i;
						m_NextKey=i+1;
					}
				}
			} 
			else
			{
				m_Backing=true;
				ResetTime();
			}
		}
	}
}
 
void CCameraKeyController::SetCamera(CCamera *Camera) const
{
	Camera->SetFOV(m_Keys[m_CurrentKey]->m_CameraInfo.m_FOV);
	//Camera->SetAspectRatio(16.0f/9.0f);
	Camera->SetPosition(m_NewPosition);
	Camera->SetZNear(m_NewNearPlane);
	Camera->SetZFar(m_NewFarPlane);
	Camera->SetLookAt(m_NewLookAt);
	Camera->SetUp(Vect3f(0.0f,1.0f,0.0f));
	//Camera->SetUp(m_Keys[m_CurrentKey]->m_CameraInfo.m_Up);
	Camera->SetMatrixs();
}

CCameraKeyController::CCameraKeyController(CXMLTreeNode &XMLTreeNode)
: m_Cycle(true)
, m_Reverse(false)
, m_Backing(false)
, m_CurrentKey(0)
, m_NextKey(1)
, m_CurrentTime(0)
, m_TotalTime(0)
, m_NewPosition(v3fZERO)
{
	LoadXML(XMLTreeNode.GetPszProperty("filename",""));
}

void CCameraKeyController::Update(float ElapsedTime) 
{
	//TODO
	m_CurrentTime+=ElapsedTime;
	GetCurrentKey();
	
	float l_StartTime=m_Keys[m_CurrentKey]->m_Time;
	float l_EndTime=m_Keys[m_CurrentKey+1]->m_Time;

	float l_timeDiff = (ElapsedTime - l_StartTime) / (l_EndTime - l_StartTime);
	
	Vect3f l_Pos=m_Keys[m_CurrentKey]->m_CameraInfo.m_Position;
	m_NewPosition = (l_Pos.Lerp(m_Keys[m_CurrentKey+1]->m_CameraInfo.m_Position,m_CurrentTime));
	
	Vect3f l_LookAt=m_Keys[m_CurrentKey]->m_CameraInfo.m_LookAt;
	m_NewLookAt = (l_LookAt.Lerp(m_Keys[m_CurrentKey+1]->m_CameraInfo.m_LookAt,m_CurrentTime));
	
	m_NewFarPlane= ((m_Keys[m_CurrentKey]->m_CameraInfo.m_FarPlane * (1 - l_timeDiff)) + ( m_Keys[m_CurrentKey]->m_CameraInfo.m_FarPlane * l_timeDiff));
	
	m_NewNearPlane= ((m_Keys[m_CurrentKey]->m_CameraInfo.m_NearPlane * (1 - l_timeDiff)) + ( m_Keys[m_CurrentKey]->m_CameraInfo.m_NearPlane * l_timeDiff));

	//= ((m_Keys[m_CurrentKey]->m_CameraInfo.m_Position * (1 - l_timeDiff)) + (m_Keys[m_CurrentKey+1]->m_Time * l_timeDiff));
	
}

void CCameraKeyController::SetCurrentTime(float CurrentTime)
{
	m_CurrentTime=CurrentTime;
}

void CCameraKeyController::ResetTime()
{
	m_CurrentTime=0.0f;
}

float CCameraKeyController::GetTotalTime()
{
	return m_TotalTime;
}

bool CCameraKeyController::IsCycle() const
{
	return m_Cycle;
}

void CCameraKeyController::SetCycle(bool Cycle)
{
	m_Cycle=Cycle;
}

bool CCameraKeyController::IsReverse() const
{
	return m_Reverse;
}

void CCameraKeyController::SetReverse(bool Reverse)
{
	m_Reverse=Reverse; 
}

CCameraKeyController::EType CCameraKeyController::GetType() const
{
	return CCameraKeyController::EType::KEY;
}

