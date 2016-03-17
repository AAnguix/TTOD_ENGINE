#include "GUIManager.h"
#include "XML\XMLTreeNode.h"

CGUIManager::CGUIManager() : m_Filename(""),
m_ActiveItem(""),
m_HotItem(""),
m_InputUpToDate(false),
m_MouseX(0),
m_MouseY(0)

{
	
}

CGUIManager::~CGUIManager()
{

}

void CGUIManager::Load(const std::string &Filename)
{
	m_Filename=Filename;

	CXMLTreeNode l_XML;

	if (l_XML.LoadFile(Filename.c_str()))
	{
		CXMLTreeNode l_GuiElements = l_XML["gui_elements"];

		if (l_GuiElements.Exists())
		{
			for (int i = 0; i < l_GuiElements.GetNumChildren(); ++i)
			{
				CXMLTreeNode l_Element = l_GuiElements(i);

				if (l_Element.GetName() == std::string("gui_spritemap"))
				{
					//SSpriteMapInfo l_SpriteInfo;

				}
			}
		}
	}
}

void CGUIManager::Reload()
{
	//Destroy();
	Load(m_Filename);
}
 


bool CGUIManager::DoButton(const std::string& GuiID, const std::string &ButtonID, const SGUIPosition& Position)
{
	bool l_Result = false;

	if (m_ActiveItem == GuiID)
	{
		if (m_MouseWentReleased)
		{
			if (m_HotItem == GuiID)
			{
				l_Result = true;
			}
			SetNotActive();
		}
	}
	else if (m_HotItem == GuiID)
	{
		if (m_MouseWentPressed)
		{
			SetActive(GuiID);
		}
	}

	if (IsMouseInside(m_MouseX, m_MouseY, Position.x, Position.y, Position.width, Position.height))
	{
		SetHot(GuiID);
	}
	else
	{
		SetNotHot(GuiID);
	}
}

SSliderResult CGUIManager::DoSlider(const std::string& GuiID, const std::string& SliderID, const SGUIPosition& Position, float MinValue, float MaxValue, float CurrentValue)
{

}


void CGUIManager::SetActive(const std::string& ID)
{
	m_ActiveItem = ID;
}

void CGUIManager::SetNotActive(const std::string& ID)
{
	m_ActiveItem = "";
}

void CGUIManager::SetHot(const std::string& ID)
{
	if (m_ActiveItem == "" || m_ActiveItem == ID)
		m_HotItem = ID;
}

void CGUIManager::SetNotHot(const std::string& ID)
{
	if (m_HotItem == ID)
		m_HotItem = "";
}

void CGUIManager::CheckInput()
{
	if (m_InputUpToDate)
	{
		CMouseInput* l_Mouse = CEngine::GetSingleton().GetInputManager()->GetMouse();
		m_MouxeX = l_Mouse->GetX();
		m_MouxeY = l_Mouse->GetY();

		m_MouseWentPressed = l_Mouse->LeftButtonBecomesPressed();
		m_MouseWentReleased = l_Mouse->LeftButtonBecomesReleased();

		m_InputUpToDate = true;
	}
}

void CGUIManager::Render(CRenderManager* RenderManager)
{
	POINT l_Cursor;
	if (GetCursorPos(&cursor))
	{
		if (ScreenToClient(CEngine::GetHWND(), &cursor))
		{
			m_X = l_Cursor.x;
			m_Y = l_Cursor.y;
		}
	}
}