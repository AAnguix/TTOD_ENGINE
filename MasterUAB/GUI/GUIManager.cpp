#include "GUIManager.h"
#include "XML\XMLTreeNode.h"
#include <assert.h>
#include "Engine.h"
#include "Render\RenderManager.h"
#include "Input\InputManagerImplementation.h"
#include "Input\MouseInput.h"
#include "Log.h"
#include "Materials\MaterialManager.h"
#include <iterator>
#include <sstream>
#include "RenderableObjects\RenderableObjectTechnique.h"

CGUIManager::CGUIManager() :
m_Filename(""),
m_ActiveItem(""),
m_HotItem(""),
m_InputUpToDate(false),
m_MouseX(0),
m_MouseY(0),
m_CurrentVertex(0),
m_CurrentSpriteMap(nullptr),
m_MouseWentPressed(false),
m_MouseWentReleased(false)

{
	
}

void CGUIManager::Initialize(unsigned int ScreenWidth, unsigned int ScreenHeight)
{
	m_ScreenHeight = ScreenHeight;
	m_ScreenWidth = ScreenWidth;
}

CGUIManager::~CGUIManager()
{

	for (size_t i = 0; i<m_VertexBuffers.size(); ++i)
	{
		delete m_VertexBuffers[i];
	}

	m_VertexBuffers.clear();
	m_Materials.clear();

	std::map<std::string, SSliderInfo*>::iterator itSlider;
	for (itSlider = m_Sliders.begin(); itSlider != m_Sliders.end(); ++itSlider)
	{
		CHECKED_DELETE(itSlider->second);
	}
	m_Sliders.clear();

	std::map<std::string, SButtonInfo*>::iterator itButton;
	for (itButton = m_Buttons.begin(); itButton != m_Buttons.end(); ++itButton)
	{
		CHECKED_DELETE(itButton->second);
	}
	m_Buttons.clear();

	std::map<std::string, SSpriteMapInfo*>::iterator itMap;
	for (itMap = m_SpriteMaps.begin(); itMap != m_SpriteMaps.end(); ++itMap)
	{
		CHECKED_DELETE(itMap->second);
	}
	m_SpriteMaps.clear();

	std::map<std::string, SSpriteInfo*>::iterator itSprite;
	for (itSprite = m_Sprites.begin(); itSprite != m_Sprites.end(); ++itSprite)
	{
		CHECKED_DELETE(itSprite->second);
	}
	m_Sprites.clear();
}

bool CGUIManager::IsMouseInside(unsigned int MouseX, unsigned int MouseY, unsigned int X, unsigned int Y, unsigned int Width, unsigned int Height)
{
	if ((MouseX >= X && MouseX <= (X + Width)) && (MouseY >= Y && MouseY <= (Y + Height)))
		return true;

	return false;
}

void CGUIManager::SortCommands()
{
	size_t l_Elements = m_SpriteMaps.size();
	//std::vector<unsigned int> m_CommandsPerSpriteMap;
	std::map<std::string, unsigned int> m_CommandsPerSpriteMap;
	std::string l_SPName;

	/*1*/
	std::map<std::string, SSpriteInfo*>::iterator itSprite;
	for (itSprite = m_Sprites.begin(); itSprite != m_Sprites.end(); ++itSprite)
	{
		l_SPName = itSprite->second->spriteMap->name;
		std::map<std::string, unsigned int>::iterator it;
		it = m_CommandsPerSpriteMap.find(l_SPName);
		if (it == m_CommandsPerSpriteMap.end())
			m_CommandsPerSpriteMap[l_SPName] = 0; /*Executed one twice per spriteMap*/

		for (size_t i = 0; i < m_Commands.size(); ++i)
		{
			if (m_Commands[i].sprite == itSprite->second)
			{
				m_CommandsPerSpriteMap[l_SPName]++;
			}
		}
	}

	/*2*/
	int l_InitialIndex = 0;
	std::map<std::string, unsigned int>::iterator it;
	for (it = m_CommandsPerSpriteMap.begin(); it != m_CommandsPerSpriteMap.end(); ++it)
	{
		it->second = l_InitialIndex;
		l_InitialIndex = l_InitialIndex + it->second;
	}

	/*3*/
	for (size_t i = 0; i < m_Commands.size(); ++i)
	{
		std::string l_SPName = m_Commands[i].sprite->spriteMap->name;
		
		std::map<std::string, unsigned int>::iterator it;
		it = m_CommandsPerSpriteMap.find(l_SPName);

		m_CommandsExecutionOrder[i] = it->second;

		it->second = it->second + 1;
	}
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
					AddSpriteMap(l_Element);
				}
			}
		}
	}
}

void CGUIManager::AddSpriteMap(CXMLTreeNode &TreeNode)
{
	std::string l_SpriteMapName = TreeNode.GetPszProperty("name");
	int l_Width = TreeNode.GetIntProperty("width");
	int l_Height = TreeNode.GetIntProperty("height");

	std::string l_MaterialName = TreeNode.GetPszProperty("material");
	CMaterial* l_Material = CEngine::GetSingleton().GetMaterialManager()->GetResource(l_MaterialName);
	m_Materials.push_back(l_Material);

	SSpriteMapInfo* l_SpriteMapInfo = new SSpriteMapInfo(l_SpriteMapName, m_Materials.size() - 1, l_Width, l_Height);

	for (int i = 0; i < TreeNode.GetNumChildren(); ++i)
	{
		CXMLTreeNode l_Element = TreeNode(i);

		if (l_Element.GetName() == std::string("sprite"))
		{
			std::string l_SpriteName = l_Element.GetPszProperty("name");
			float l_x = l_Element.GetFloatProperty("x", 0.0f);
			float l_y = l_Element.GetFloatProperty("y", 0.0f);
			float l_w = l_Element.GetFloatProperty("w", 0.0f);
			float l_h = l_Element.GetFloatProperty("h", 0.0f);
			
			float l_u1 = l_x / (float)l_Width;
			float l_u2 = (l_x + l_w) / (float)l_Width;
			float l_v1 = l_y / (float)l_Height;
			float l_v2 = (l_y + l_h) / (float)l_Height;

			SSpriteInfo* l_Sprite = new SSpriteInfo(l_SpriteMapInfo,l_u1,l_u2,l_v1,l_v2);
			m_Sprites.insert(std::pair<std::string, SSpriteInfo*>(l_SpriteName, l_Sprite));
		}
	}

	m_SpriteMaps.insert(std::pair<std::string, SSpriteMapInfo*>(l_SpriteMapName, l_SpriteMapInfo));
	CRenderableVertexs *l_RV = new CTrianglesListRenderableVertexs<MV_POSITION4_COLOR_TEXTURE_VERTEX>(m_CurrentBufferData, s_MaxVerticesPerCall, 10, true);
	m_VertexBuffers.push_back(l_RV);
}

void CGUIManager::Reload()
{
	Destroy();
	Load(m_Filename);
}

void CGUIManager::Destroy()
{
	for (size_t i = 0; i<m_VertexBuffers.size(); ++i)
	{
		delete m_VertexBuffers[i];
	}

	m_VertexBuffers.clear();
	m_Materials.clear();

	std::map<std::string, SSliderInfo*>::iterator itSlider;
	for (itSlider = m_Sliders.begin(); itSlider != m_Sliders.end(); ++itSlider)
	{
		CHECKED_DELETE(itSlider->second);
	}
	m_Sliders.clear();

	std::map<std::string, SButtonInfo*>::iterator itButton;
	for (itButton = m_Buttons.begin(); itButton != m_Buttons.end(); ++itButton)
	{
		CHECKED_DELETE(itButton->second);
	}
	m_Buttons.clear();

	std::map<std::string, SSpriteMapInfo*>::iterator itMap;
	for (itMap = m_SpriteMaps.begin(); itMap != m_SpriteMaps.end(); ++itMap)
	{
		CHECKED_DELETE(itMap->second);
	}
	m_SpriteMaps.clear();

	std::map<std::string, SSpriteInfo*>::iterator itSprite;
	for (itSprite = m_Sprites.begin(); itSprite != m_Sprites.end(); ++itSprite)
	{
		CHECKED_DELETE(itSprite->second);
	}
	m_Sprites.clear();
}

CGUIManager::SSpriteInfo* CGUIManager::GetSprite(const std::string& Name)
{
	std::map<std::string, SSpriteInfo*>::iterator itMap;

	itMap = m_Sprites.find(Name);

	if (itMap == m_Sprites.end())
		return nullptr;
	else
		return itMap->second;
}

CGUIManager::SSliderInfo*  CGUIManager::GetSlider(const std::string& SliderID)
{
	std::map<std::string, SSliderInfo*>::iterator itMap;

	itMap = m_Sliders.find(SliderID);

	if (itMap == m_Sliders.end())
		return nullptr;
	else
		return itMap->second;
}

CGUIManager::SButtonInfo* CGUIManager::GetButton(const std::string& ButtonID)
{
	std::map<std::string, SButtonInfo*>::iterator itMap;

	itMap = m_Buttons.find(ButtonID);

	if (itMap == m_Buttons.end())
		return nullptr;
	else
		return itMap->second;
}

void CGUIManager::AddButton(const std::string& ButtonID, const std::string& Normal, const std::string& Highlight, const std::string& Pressed)
{
	SSpriteInfo* l_Normal = GetSprite(Normal);
	SSpriteInfo* l_Highlight = GetSprite(Highlight);
	SSpriteInfo* l_Pressed = GetSprite(Pressed);
	assert(l_Normal != nullptr);
	assert(l_Highlight != nullptr);
	assert(l_Pressed != nullptr);

	SButtonInfo* l_Button = new SButtonInfo(l_Normal, l_Highlight, l_Pressed);
	m_Buttons.insert(std::pair<std::string, SButtonInfo*>(ButtonID, l_Button));
}

bool CGUIManager::DoButton(const std::string& GuiID, const std::string &ButtonID, const SGUIPosition& Position)
{
	CheckInput(); 

	SButtonInfo* l_Button = GetButton(ButtonID);
	SSpriteInfo* l_Sprite = l_Button->normal;
	
	bool l_Result = false;

	if (m_ActiveItem == GuiID)
	{
		l_Sprite = l_Button->pressed;

		if (m_MouseWentReleased)
		{
			if (m_HotItem == GuiID)
			{
				l_Result = true;
			}
			SetNotActive(GuiID);
		}
	}
	else if (m_HotItem == GuiID)
	{
		l_Sprite = l_Button->highlight;

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

	{
		SGUICommand l_Command = { l_Sprite, Position.x, Position.y, Position.x + Position.width, Position.y + Position.height
		, 0, 0, 1, 1,
		CColor(1.0f,1.0f,1.0f,1.0f) };
		m_Commands.push_back(l_Command);
	}

	return true;
}

void CGUIManager::AddSlider(const std::string& SliderID, const std::string& Base, const std::string& Top, const std::string& Handle, const std::string& PressedHandle)
{
	SSpriteInfo* l_Base = GetSprite(Base);
	SSpriteInfo* l_Top = GetSprite(Top);
	SSpriteInfo* l_Handle = GetSprite(Handle);
	SSpriteInfo* l_PressedHandle = GetSprite(PressedHandle);
	
	assert(l_Base != nullptr);
	assert(l_Top != nullptr);
	assert(l_Handle != nullptr);
	assert(l_PressedHandle != nullptr);

	SSliderInfo* l_Slider = new SSliderInfo(l_Base, l_Top, l_Handle, l_PressedHandle);
	m_Sliders.insert(std::pair<std::string, SSliderInfo*>(SliderID, l_Slider));
}

CGUIManager::SSliderResult CGUIManager::DoSlider(const std::string& GuiID, const std::string& SliderID, const SGUIPosition& Position, float MinValue, float MaxValue, float CurrentValue)
{
	SSliderInfo* l_SliderInfo = GetSlider(SliderID);
	SSliderResult l_Result;
	l_Result.real = .0f;
	l_Result.temp = .0f;

	if (l_SliderInfo != nullptr)
	{
		CheckInput();
		bool RealResult = false;

		float l_Factor = (float)(m_MouseX - Position.x) / ((float)Position.width);
		if (l_Factor < 0) l_Factor = 0;
		else if (l_Factor > 1) l_Factor = 1;

		l_Result.temp = MinValue + (MaxValue - MinValue)*l_Factor;

		if (m_ActiveItem == GuiID)
		{
			if (m_MouseWentReleased)
			{
				if (m_HotItem == GuiID)
				{
					RealResult = true;
				}
				SetNotActive(GuiID);
			}
		}
		else if (m_HotItem == GuiID)
		{
			if (m_MouseWentPressed)
			{
				SetActive(GuiID);
			}
		}

		if (RealResult)
		{
			l_Result.real = l_Result.temp;
		}
		else if (m_ActiveItem == GuiID)
		{
			l_Result.real = CurrentValue;
		}
		else
		{
			l_Result.temp = CurrentValue;
			l_Result.real = CurrentValue;
		}

		float l_HandlePosition = Position.x + Position.width * (l_Result.temp - MinValue) / (MaxValue - MinValue);
		float l_RealHandleWidth = l_SliderInfo->handleRelativeWidth * Position.width;
		float l_RealHandleHeight = l_SliderInfo->handleRelativeHeight * Position.height;

		int l_RealHandleX = (int)(l_HandlePosition - l_RealHandleWidth * 0.5f);
		int l_RealHandleY = (int)(Position.y + Position.height * 0.5f - l_RealHandleHeight * 0.5);

		if (IsMouseInside(m_MouseX, m_MouseY, Position.x, Position.y, Position.width, Position.height))
		{
			SetHot(GuiID);
		}
		else if (IsMouseInside(m_MouseX, m_MouseY, l_RealHandleX, l_RealHandleY, int(l_RealHandleWidth), int(l_RealHandleHeight)))
		{
			SetHot(GuiID);
		}
		else
		{
			SetNotHot(GuiID);
		}

		SGUICommand l_Base = { l_SliderInfo->base, Position.x, Position.y, Position.x + Position.width, Position.y + Position.height
			, 0, 0, 1, 1,
			CColor(1.0f, 1.0f, 1.0f, 1.0f) };
		m_Commands.push_back(l_Base);

		SGUICommand l_Top = { l_SliderInfo->top, Position.x, Position.y, (int)l_HandlePosition, Position.y + Position.height,
			0, 0, (l_Result.temp - MinValue) / (MaxValue - MinValue), 1,
			CColor(1.0f, 1.0f, 1.0f, 1.0f) };
		m_Commands.push_back(l_Top);

		SGUICommand l_Handle = {
			(m_ActiveItem == GuiID && m_HotItem == GuiID) ? l_SliderInfo->pressed_handle : l_SliderInfo->handle,
			l_RealHandleX, l_RealHandleY, l_RealHandleX + (int)l_RealHandleWidth, l_RealHandleY + (int)l_RealHandleHeight,
			0, 0, 1, 1,
			CColor(1.0f, 1.0f, 1.0f, 1.0f) };
		m_Commands.push_back(l_Handle);
	}

	return l_Result;
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
	if ("" == m_ActiveItem || ID == m_ActiveItem)
		m_HotItem = ID;
}

void CGUIManager::SetNotHot(const std::string& ID)
{
	if (m_HotItem == ID)
		m_HotItem = "";
}

void CGUIManager::CheckInput()
{
	if (!m_InputUpToDate)
	{
		CMouseInput* l_Mouse = CEngine::GetSingleton().GetInputManager()->GetMouse();
		m_MouseX = l_Mouse->GetX();
		m_MouseY = l_Mouse->GetY();

		m_MouseWentPressed = l_Mouse->LeftButtonBecomesPressed();
		m_MouseWentReleased = l_Mouse->LeftButtonBecomesReleased();

		m_InputUpToDate = true;
	}
}

void CGUIManager::Render(CRenderManager* RenderManager)
{
	m_InputUpToDate = false;

	//m_VertexBuffers = new CPointListRenderableVertexs<MV_PARTICLE_VERTEX>(m_ParticleRenderableData, s_MaxParticlesPerInstance, s_MaxParticlesPerInstance, true);
	//l_RV = new CTriangleListRenderableIndexed16Vertexs<MV_POSITION_WEIGHT_INDICES_NORMAL_TEXTURE_VERTEX>(l_VtxsData, l_NumVertexs, l_IdxData, m_NumIndexs, false);

	for (size_t i = 0; i < m_Commands.size(); ++i)
	{
		SGUICommand &l_Command = m_Commands[i];
		//SGUICommand &l_Command = m_Commands[m_CommandsExecutionOrder[i]];
		assert(l_Command.x1 <= l_Command.x2);
		assert(l_Command.y1 <= l_Command.y2);

		SSpriteInfo* l_CommandSprite = l_Command.sprite;
		SSpriteMapInfo* l_CommandSpriteMap = l_CommandSprite->spriteMap;
		
		if (i==0)
			m_CurrentSpriteMap = l_CommandSpriteMap;

		float x1 = (l_Command.x1 / (m_ScreenWidth * 0.5f)) - 1.0f;
		float x2 = (l_Command.x2 / (m_ScreenWidth * 0.5f)) - 1.0f;
		float y1 = 1.0f - (l_Command.y1 / (m_ScreenHeight * 0.5f));
		float y2 = 1.0f - (l_Command.y2 / (m_ScreenHeight * 0.5f));

		float u1 = l_CommandSprite->u1 * (1.0f - l_Command.u1) + l_CommandSprite->u2 * l_Command.u1;
		float u2 = l_CommandSprite->u1 * (1.0f - l_Command.u2) + l_CommandSprite->u2 * l_Command.u2;
		float v1 = l_CommandSprite->v1 * (1.0f - l_Command.v1) + l_CommandSprite->v2 * l_Command.v1;
		float v2 = l_CommandSprite->v1 * (1.0f - l_Command.v2) + l_CommandSprite->v2 * l_Command.v2;

		m_CurrentBufferData[m_CurrentVertex++] = { Vect4f(x1, y2, 0.f, 1.f), l_Command.color, Vect2f(u1, v2) };
		m_CurrentBufferData[m_CurrentVertex++] = { Vect4f(x2, y2, 0.f, 1.f), l_Command.color, Vect2f(u2, v2) };
		m_CurrentBufferData[m_CurrentVertex++] = { Vect4f(x1, y1, 0.f, 1.f), l_Command.color, Vect2f(u1, v1) };

		m_CurrentBufferData[m_CurrentVertex++] = { Vect4f(x1, y1, 0.f, 1.f), l_Command.color, Vect2f(u1, v1) };
		m_CurrentBufferData[m_CurrentVertex++] = { Vect4f(x2, y2, 0.f, 1.f), l_Command.color, Vect2f(u2, v2) };
		m_CurrentBufferData[m_CurrentVertex++] = { Vect4f(x2, y1, 0.f, 1.f), l_Command.color, Vect2f(u2, v1) };

		if (m_CurrentSpriteMap != l_CommandSpriteMap || m_CurrentVertex == s_MaxVerticesPerCall)
		{
			if (m_CurrentVertex > 0)
			{
				if (m_CurrentVertex > s_MaxVerticesPerCall)
					CEngine::GetSingleton().GetLogManager()->Log("GUI warning. CurrentVertex equal to s_MaxVerticesPerCall");	

				CMaterial* l_Material = m_Materials[m_CurrentSpriteMap->materialIndex];
				l_Material->Apply();

				m_VertexBuffers[m_CurrentSpriteMap->materialIndex]->UpdateVertexs(m_CurrentBufferData, m_CurrentVertex);
				m_VertexBuffers[m_CurrentSpriteMap->materialIndex]->Render(RenderManager, l_Material->GetRenderableObjectTechnique()->GetEffectTechnique(), &CEffectManager::m_SceneEffectParameters, m_CurrentVertex);
			}

			m_CurrentVertex = 0;
			m_CurrentSpriteMap = l_CommandSpriteMap;
		}
	}

	if (m_CurrentVertex > 0)
	{
		if (m_CurrentVertex > s_MaxVerticesPerCall)
			CEngine::GetSingleton().GetLogManager()->Log("GUI warning. CurrentVertex equal to s_MaxVerticesPerCall");

		CMaterial* l_Material = m_Materials[m_CurrentSpriteMap->materialIndex];
		l_Material->Apply();

		m_VertexBuffers[m_CurrentSpriteMap->materialIndex]->UpdateVertexs(m_CurrentBufferData, m_CurrentVertex);
		m_VertexBuffers[m_CurrentSpriteMap->materialIndex]->Render(RenderManager, l_Material->GetRenderableObjectTechnique()->GetEffectTechnique(), &CEffectManager::m_SceneEffectParameters, m_CurrentVertex);
		m_CurrentVertex = 0;
	}

	m_Commands.clear();
}

