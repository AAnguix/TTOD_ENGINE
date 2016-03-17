#include "Cinematics\Cinematic.h"
#include "Utils\Utils.h"
#include "Components\ComponentManager.h"

CCinematic::CCinematic(CXMLTreeNode &TreeNode)
{
	SetName(TreeNode.GetPszProperty("name"));
	m_FileName=TreeNode.GetPszProperty("file");
	m_Cycle=true;
}

CCinematic::~CCinematic()
{
	for(size_t i=0;i<m_CinematicObjects.size();++i)
	{
		CHECKED_DELETE(m_CinematicObjects[i]);
	}
	m_CinematicObjects.clear();
}

void CCinematic::Stop()
{	
	Pause();
	m_CurrentTime=0;

	for(size_t i=0;i<m_CinematicObjects.size();++i)
	{
		m_CinematicObjects[i]->Stop();
	}
}

void CCinematic::Play(bool Cycle)
{	
	m_Cycle=Cycle;
	m_Playing=true;

	for(size_t i=0;i<m_CinematicObjects.size();++i)
	{
		m_CinematicObjects[i]->Play(Cycle);
	}
}

void CCinematic::Pause()
{
	m_Playing=false;
}

void CCinematic::LoadXML(const std::string &Filename)
{
	m_FileName=Filename;
	CXMLTreeNode l_XML;

	if (l_XML.LoadFile(Filename.c_str()))
	{
		CXMLTreeNode l_Cinematic = l_XML["cinematic"];

		if (l_Cinematic.Exists())
		{
			m_Duration=l_Cinematic.GetFloatProperty("duration",0.0f);

			for (int i = 0; i < l_Cinematic.GetNumChildren(); ++i)
			{
				CXMLTreeNode l_Element = l_Cinematic(i);

				if (l_Element.GetName() == std::string("cinematic_object"))
				{
					CCinematicObject *l_CinematicObject = new CCinematicObject(l_Element);
					m_CinematicObjects.push_back(l_CinematicObject);
				}
			}
		}
	}
}

void CCinematic::AddCinematicObject(CCinematicObject *CinematicObject)
{
	m_CinematicObjects.push_back(CinematicObject);
}

void CCinematic::Update(float ElapsedTime)
{
	if(m_Playing)
	{
		m_CurrentTime+=ElapsedTime;
		
		if(m_CurrentTime>m_Duration)
		{
			if(m_Cycle) 
			{
				m_CurrentTime=0;
				for(size_t i=0;i<m_CinematicObjects.size();++i)
				{
					m_CinematicObjects[i]->OnRestartCycle();
				}
			}
			else Stop();
		}
		else
		{
			for(size_t i=0;i<m_CinematicObjects.size();++i)
			{
				m_CinematicObjects[i]->Update(ElapsedTime);
			}
		}
	}
}

void CCinematic::Render(CRenderManager *RM){}