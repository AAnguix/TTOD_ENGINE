#include "GUIManager.h"
#include "XML\XMLTreeNode.h"
#include <assert.h>
#include "Engine.h"
#include "Render\RenderManager.h"
#include "Input\InputManagerImplementation.h"
#include "Input\MouseInput.h"
#include "Log\Log.h"
#include "Materials\MaterialManager.h"
#include <iterator>
#include <sstream>
#include "RenderableObjects\RenderableObjectTechnique.h"
#include "Engine.h"
#include "LuabindManager\LuabindManager.h"
#include "Input\KeyBoardInput.h"

CGUIManager::CGUIManager()
:m_ActiveItem("")
,m_HotItem("")
,m_SelectedItem("")
,m_InputUpToDate(false)
,m_MouseX(0)
,m_MouseY(0)
,m_MouseWentPressed(false)
,m_MouseWentReleased(false)
,m_AnimationTimer(0.0f)
{
	
}

void CGUIManager::Initialize(float ScreenWidth, float ScreenHeight)
{
	m_ScreenHeight = ScreenHeight;
	m_ScreenWidth = ScreenWidth;
}

CGUIManager::~CGUIManager()
{
	Destroy();
}

bool CGUIManager::IsMouseInside(float MouseX, float MouseY, float X, float Y, float Width, float Height)
{
	if ((MouseX >= X && MouseX <= (X + Width)) && (MouseY >= Y && MouseY <= (Y + Height)))
		return true;

	return false;
}

void CGUIManager::SortCommands()
{
	size_t l_Elements = m_SpriteMaps.size();
	//std::vector<unsigned int> m_CommandsPerSpriteMap;
	std::vector<unsigned int> m_CommandsPerSpriteMap(m_Materials.size());
	std::string l_SPName;

	/*1*/
	for (size_t i = 0; i < m_Commands.size(); ++i)
	{
		if (m_Commands[i].sprite!=nullptr)
			m_CommandsPerSpriteMap[m_Commands[i].sprite->spriteMap->materialIndex]++;
	}

	/*2*/
	unsigned int l_InitialIndex = 0;
	for (unsigned int i = 0; i < m_CommandsPerSpriteMap.size(); ++i)
	{
		unsigned int l_Value = l_InitialIndex + m_CommandsPerSpriteMap[i];
		m_CommandsPerSpriteMap[i] = l_InitialIndex;
		l_InitialIndex = l_Value;
	}

	/*3*/
	for (size_t i = 0; i < m_Commands.size(); ++i)
	{
		if (m_Commands[i].sprite!=nullptr)
			m_CommandsExecutionOrder.push_back(0);
	}
	for (size_t i = 0; i < m_Commands.size(); ++i)
	{
		SSpriteInfo* l_Sprite = m_Commands[i].sprite;

		if (l_Sprite != nullptr)
		{
			unsigned int l_MatIndex = l_Sprite->spriteMap->materialIndex;

			unsigned int l_Index = m_CommandsPerSpriteMap[l_MatIndex];

			m_CommandsExecutionOrder[l_Index] = i;

			m_CommandsPerSpriteMap[l_MatIndex]++;
		}
	}
}

CGUIManager::SGUIPosition::SGUIPosition(float X, float Y, float Width, float Height, GUIAnchor Anchor, GUICoordType AnchorCoordsType, GUICoordType SizeCoordsType)
{
	float l_W = CEngine::GetSingleton().GetGUIManager()->GetScreenWidth();
	float l_H = CEngine::GetSingleton().GetGUIManager()->GetScreenHeight();

	switch (SizeCoordsType)
	{
		default:
		assert(false);
		case GUICoordType::GUI_ABSOLUTE:
			width = Width;
			height = Height;
			break;
		case GUICoordType::GUI_RELATIVE:
			width = Width * l_W;
			height = Height * l_H;
			if (X < 0){ X = 1.0f + X; }
			if (Y < 0){ Y = 1.0f + Y; }
			break;
			case GUICoordType::GUI_RELATIVE_WIDTH:
			width = Width * l_W;
			height = Height * l_W;
			if (X < 0){ X = 1.0f + X; }
			if (Y < 0){ Y = 1.0f + Y; }
			break;
		case GUICoordType::GUI_RELATIVE_HEIGHT:
			width = Width * l_H;
			height = Height * l_H;
			if (X < 0){ X = 1.0f + X; }
			if (Y < 0){ Y = 1.0f + Y; }
			break;
	}

	float unitPixelSizeX = 0.0f;
	float unitPixelSizeY = 0.0f;

	

	switch (AnchorCoordsType)
	{
		default:
		assert(false);
		case GUICoordType::GUI_ABSOLUTE:
			unitPixelSizeX = 1;
			unitPixelSizeY = 1;
			break;
		case GUICoordType::GUI_RELATIVE:
			unitPixelSizeX = l_W;
			unitPixelSizeY = l_H;
			break;
		case GUICoordType::GUI_RELATIVE_WIDTH:
			unitPixelSizeX = l_W;
			unitPixelSizeY = l_W;
			break;
		case GUICoordType::GUI_RELATIVE_HEIGHT:
			unitPixelSizeX = l_H;
			unitPixelSizeY = l_H;
			break;
	}

	if ((int)Anchor & (int)GUIAnchor::LEFT)
	{
		x = (X * unitPixelSizeX);
	}
	else if ((int)Anchor & (int)GUIAnchor::CENTER)
	{
		x = ((X * unitPixelSizeX) - (width*0.5f));
	}
	else if ((int)Anchor & (int)GUIAnchor::RIGHT)
	{
		x = (X * unitPixelSizeX - width);
	}
	else { assert(false); }
	if (x < 0)
		x += l_W;

	if ((int)Anchor & (int)GUIAnchor::TOP)
	{
		y = (Y * unitPixelSizeY);
	}
	else if (Anchor & (int)GUIAnchor::MID)
	{
		y = ((Y * unitPixelSizeY) - (height*0.5f));
	}
	else if (Anchor & (int)GUIAnchor::BOTTOM)
	{
		y = (Y * unitPixelSizeY - height);
	}
	else { assert(false); }
	if (y < 0)
		y += l_H;
}

CGUIManager::SGUIPosition::SGUIPosition(float X, float Y, float Width, float Height)
{
	x = X;
	y = Y;
	width = Width;
	height = Height;
}

void CGUIManager::Load(const std::string &Filename)
{
	bool l_Found = false;
	for (size_t i = 0; i < m_Filenames.size(); ++i)
	{
		if (m_Filenames[i] == Filename)
			l_Found = true;
	}
	if (!l_Found)
		m_Filenames.push_back(Filename);

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
		else { assert(false); }
	} 
	else { assert(false); }
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
	for (size_t i = 0; i < m_Filenames.size(); ++i)
	{
		Load(m_Filenames[i]);
	}
}

void CGUIManager::Destroy()
{
	SetNotActive(m_ActiveItem);
	SetNotHot(m_HotItem);
	SetNotSelected(m_SelectedItem);

	m_MouseWentPressed = false;
	m_MouseWentReleased = false;

	for (size_t i = 0; i<m_VertexBuffers.size(); ++i)
	{
		delete m_VertexBuffers[i];
	}

	m_Commands.clear();
	m_VertexBuffers.clear();
	m_Materials.clear();

	std::map<std::string, SSliderInfo*>::iterator itSlider;
	for (itSlider = m_Sliders.begin(); itSlider != m_Sliders.end(); ++itSlider)
	{
		CHECKED_DELETE(itSlider->second);
	}
	m_Sliders.clear();

	std::map<std::string, SHealthBarInfo*>::iterator itHealthBar;
	for (itHealthBar = m_HealthBars.begin(); itHealthBar != m_HealthBars.end(); ++itHealthBar)
	{
		CHECKED_DELETE(itHealthBar->second);
	}
	m_HealthBars.clear();

	std::map<std::string, SImageInfo*>::iterator itImage;
	for (itImage = m_Images.begin(); itImage != m_Images.end(); ++itImage)
	{
		CHECKED_DELETE(itImage->second);
	}
	m_Images.clear();

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

	m_LineHeightPerFont.clear();
	m_BasePerFont.clear();
	m_CharactersPerFont.clear();
	m_KerningPerFont.clear();
	m_TexturePerFont.clear();
}

CGUIManager::SSpriteInfo* CGUIManager::GetSprite(const std::string& Name)
{
	std::map<std::string, SSpriteInfo*>::iterator itMap;

	itMap = m_Sprites.find(Name);

	if (itMap == m_Sprites.end())
	{
		CEngine::GetSingleton().GetLogManager()->Log("Can't find sprite " + Name);
		assert(false);
		return nullptr;
	}
	else
		return itMap->second;
}

CGUIManager::SImageInfo* CGUIManager::GetImage(const std::string& ImageID)
{
	std::map<std::string, SImageInfo*>::iterator itMap;

	itMap = m_Images.find(ImageID);

	if (itMap == m_Images.end())
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

CGUIManager::SSliderInfo*  CGUIManager::GetSlider(const std::string& SliderID)
{
	std::map<std::string, SSliderInfo*>::iterator itMap;

	itMap = m_Sliders.find(SliderID);

	if (itMap == m_Sliders.end())
	{
		assert(false);
		return nullptr;
	}
	else
		return itMap->second;
}

CGUIManager::SHealthBarInfo* CGUIManager::GetHealthBar(const std::string& HealthBarID)
{
	std::map<std::string, SHealthBarInfo*>::iterator itMap;

	itMap = m_HealthBars.find(HealthBarID);

	if (itMap == m_HealthBars.end())
	{
		assert(false);
		return nullptr;
	}
	else
		return itMap->second;
}


void CGUIManager::AddImage(const std::string& ImageID, const std::string& Sprite)
{
	SSpriteInfo* l_Sprite = GetSprite(Sprite);
	assert(l_Sprite != nullptr);


	SImageInfo* l_Image = new SImageInfo(l_Sprite);
	m_Images.insert(std::pair<std::string, SImageInfo*>(ImageID, l_Image));
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

void CGUIManager::AddHealthBar(const std::string& HealthBarID, const std::string& Base, const std::string& Top, const std::string& Background)
{
	SSpriteInfo* l_Base = GetSprite(Base);
	SSpriteInfo* l_Top = GetSprite(Top);
	SSpriteInfo* l_Background = GetSprite(Background);

	assert(l_Base != nullptr);
	assert(l_Top != nullptr);
	assert(l_Background != nullptr);

	SHealthBarInfo* l_HealthBar = new SHealthBarInfo(l_Base, l_Top, l_Background);
	m_HealthBars.insert(std::pair<std::string, SHealthBarInfo*>(HealthBarID, l_HealthBar));
}

void CGUIManager::DoImage(const std::string& GuiID, const std::string& ImageID, const SGUIPosition& Position)
{
	CheckInput();

	SImageInfo* l_Image = GetImage(ImageID);
	SSpriteInfo* l_Sprite = l_Image->sprite;

	{
		SGUICommand l_Command = { l_Sprite, (int)Position.x, (int)Position.y, (int)(Position.x + Position.width), (int)(Position.y + Position.height)
		, 0.0f, 0.0f, 1.0f, 1.0f,
		CColor(1.0f, 1.0f, 1.0f, 1.0f) };
		m_Commands.push_back(l_Command);
	}
}

bool CGUIManager::DoButton(const std::string& GuiID, const std::string& ButtonID, const SGUIPosition& Position)
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
		SGUICommand l_Command = { l_Sprite, (int)Position.x, (int)Position.y, (int)(Position.x + Position.width), (int)(Position.y + Position.height)
		,0.0f,0.0f,1.0f,1.0f,
		CColor(1.0f,1.0f,1.0f,1.0f) };
		m_Commands.push_back(l_Command);
	}

	return l_Result;
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
		else if (IsMouseInside(m_MouseX, m_MouseY, (float)l_RealHandleX, (float)l_RealHandleY, l_RealHandleWidth, l_RealHandleHeight))
		{
			SetHot(GuiID);
		}
		else
		{
			SetNotHot(GuiID);
		}

		SGUICommand l_Base = { l_SliderInfo->base, (int)Position.x, (int)Position.y, (int)(Position.x + Position.width), (int)(Position.y + Position.height)
			, 0, 0, 1, 1,
			CColor(1.0f, 1.0f, 1.0f, 1.0f) };
		m_Commands.push_back(l_Base);

		SGUICommand l_Top = { l_SliderInfo->top, (int)Position.x, (int)Position.y, (int)l_HandlePosition, int(Position.y + Position.height),
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

void CGUIManager::DoHealthBar(const std::string& GuiID, const std::string& HealthBarID, const SGUIPosition& Position, float MinValue, float MaxValue, float CurrentValue)
{
	SHealthBarInfo* l_HealthBarInfo = GetHealthBar(HealthBarID);
	float l_CurrentValuePos = 0.0f;

	if (l_HealthBarInfo != nullptr)
	{
		CheckInput();

		if (m_ActiveItem == GuiID)
		{
			if (m_MouseWentReleased)
			{
				if (m_HotItem == GuiID)
				{
					//
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

		l_CurrentValuePos = Position.x + Position.width * (CurrentValue - MinValue) / (MaxValue - MinValue);
		float l_BarWidth = l_HealthBarInfo->barRelativeWidth * Position.width;
		float l_BarHeight = l_HealthBarInfo->barRelativeHeight * Position.height;

		if (IsMouseInside(m_MouseX, m_MouseY, Position.x, Position.y, Position.width, Position.height))
		{
			SetHot(GuiID);
		}
		else
		{
			SetNotHot(GuiID);
		}

		SGUICommand l_Background = { l_HealthBarInfo->background, (int)Position.x, (int)Position.y, (int)(Position.x + Position.width), (int)(Position.y + Position.height)
		, 0, 0, 1, 1,
		CColor(1.0f, 1.0f, 1.0f, 1.0f) };
		m_Commands.push_back(l_Background);

		SGUICommand l_Base = { l_HealthBarInfo->base, (int)Position.x, (int)Position.y, (int)(Position.x + l_BarWidth), (int)(Position.y + l_BarHeight)
		, 0, 0, 1, 1,
		CColor(1.0f, 1.0f, 1.0f, 1.0f) };
		m_Commands.push_back(l_Base);

		SGUICommand l_Top = { l_HealthBarInfo->top, (int)Position.x, (int)Position.y, (int)l_CurrentValuePos, int(Position.y + l_BarHeight),
		0, 0, (CurrentValue - MinValue) / (MaxValue - MinValue), 1,
		CColor(1.0f, 1.0f, 1.0f, 1.0f) };
		m_Commands.push_back(l_Top);
	}
}

void CGUIManager::DoText(const std::string& GuiID, const std::string& Font, const SGUIPosition& Position, const std::string& Sprite, const std::string& Text)
{
	CheckInput();

	SSpriteInfo* l_Sprite = nullptr;
	std::map<std::string, SSpriteInfo*>::iterator itMap;
	itMap = m_Sprites.find(Sprite);
	if (itMap != m_Sprites.end())
		l_Sprite = itMap->second;


	SGUICommand l_Command = { l_Sprite, (int)Position.x, (int)Position.y, (int)(Position.x + Position.width), (int)(Position.y + Position.height)
	, 0, 0, 1, 1,
	CColor(1.0f, 1.0f, 1.0f, 1.0f) };
	m_Commands.push_back(l_Command);

	FillCommandQueueWithText(Font, Text, Vect2f(Position.x + Position.width * 0.05f, Position.y + Position.height * 0.75f), GUIAnchor::BASE_LEFT);
}

std::string CGUIManager::DoTextBox(const std::string& GuiID, const std::string& Font, const SGUIPosition& Position, const std::string& Sprite, const std::string& CurrentText, float ElapsedTime)
{
	CheckInput();
	m_AnimationTimer += ElapsedTime;

	std::string l_DisplayText;
	std::string l_ActiveText = CurrentText;
	SSpriteInfo* l_Sprite = GetSprite(Sprite);

	if (m_ActiveItem == GuiID)
	{
		if (m_MouseWentReleased)
		{
			if (m_HotItem == GuiID)
			{
				SetSelected(GuiID);
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

	if (IsMouseInside(m_MouseX, m_MouseY, Position.x, Position.y, Position.width, Position.height))
	{
		SetHot(GuiID);
	}
	else
	{
		SetNotHot(GuiID);
	}

	SGUICommand l_Command = { l_Sprite, (int)Position.x, (int)Position.y, (int)(Position.x + Position.width), (int)(Position.y + Position.height)
	, 0, 0, 1, 1,
	CColor(1.0f, 1.0f, 1.0f, 1.0f) };
	m_Commands.push_back(l_Command);

	if (m_SelectedItem == GuiID)
	{
		if (m_AnimationTimer > 1.0f)
		{
			m_AnimationTimer = 0.0f;
			l_DisplayText = l_ActiveText;
		}
		else if (m_AnimationTimer > 0.5f)
		{
			l_DisplayText = l_ActiveText + "_";
		}
		else
		{
			l_DisplayText = l_ActiveText;
		}

		CKeyBoardInput* l_KeyBoard = CEngine::GetSingleton().GetInputManager()->GetKeyBoard();

		wchar_t l_LastChar = l_KeyBoard->ConsumeLastChar();

		if (l_LastChar >= 0x20 && l_LastChar < 255)
		{
			l_ActiveText += (char)l_LastChar;
		}
		else if (l_LastChar == '\r')
		{
			l_ActiveText += '\n';
		}
		else if (l_LastChar == '\b')
		{
			l_ActiveText = l_ActiveText.substr(0, l_ActiveText.length() - 1);
		}

		FillCommandQueueWithText(Font, l_DisplayText, Vect2f(Position.x + Position.width * 0.05f, Position.y + Position.height * 0.75f), GUIAnchor::BASE_LEFT);
	}

	return l_ActiveText;
}

void CGUIManager::CreateConsole(const std::string& GuiID, const std::string& Font, const SGUIPosition& Position, const std::string& Sprite, float ElapsedTime)
{
	static std::string s_Text = "";
	s_Text = CEngine::GetSingleton().GetGUIManager()->DoTextBox(GuiID, Font, Position, Sprite, "> " + s_Text, ElapsedTime);
	s_Text = s_Text.substr(2, s_Text.length() - 2);

	if (s_Text.length() > 0 && s_Text[s_Text.length() - 1] == '\n')
	{
		std::string l_Command = s_Text.substr(0, s_Text.length() - 1);
		CEngine::GetSingleton().GetLuabindManager()->RunCode(l_Command);
		s_Text = "";
	}
}

void CGUIManager::SetActive(const std::string& ID)
{
	m_ActiveItem = ID;
	m_SelectedItem = "";
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

void CGUIManager::SetSelected(const std::string& ID)
{
	m_SelectedItem = ID;
}

void CGUIManager::SetNotSelected(const std::string& ID)
{
	if (m_SelectedItem == ID)
		m_SelectedItem = "";
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
	SortCommands();

	m_InputUpToDate = false;

	//m_VertexBuffers = new CPointListRenderableVertexs<MV_PARTICLE_VERTEX>(m_ParticleRenderableData, s_MaxParticlesPerInstance, s_MaxParticlesPerInstance, true);
	//l_RV = new CTriangleListRenderableIndexed16Vertexs<MV_POSITION_WEIGHT_INDICES_NORMAL_TEXTURE_VERTEX>(l_VtxsData, l_NumVertexs, l_IdxData, m_NumIndexs, false);

	SSpriteMapInfo* l_CurrentSpriteMap = nullptr;
	int l_CurrentVertex = 0;

	for (size_t i = 0; i < m_CommandsExecutionOrder.size(); ++i)
	{
		SGUICommand &l_Command = m_Commands[m_CommandsExecutionOrder[i]];
		//SGUICommand &l_Command = m_Commands[m_CommandsExecutionOrder[i]];
		assert(l_Command.x1 <= l_Command.x2);
		assert(l_Command.y1 <= l_Command.y2);

		SSpriteInfo* l_CommandSprite = l_Command.sprite;
		SSpriteMapInfo* l_CommandSpriteMap = l_CommandSprite->spriteMap;

		if (l_CurrentSpriteMap != l_CommandSpriteMap || l_CurrentVertex == s_MaxVerticesPerCall)
		{
			if (l_CurrentVertex > 0)
			{
				//if (l_CurrentVertex == s_MaxVerticesPerCall)
					//CEngine::GetSingleton().GetLogManager()->Log("GUI warning. CurrentVertex equal to s_MaxVerticesPerCall");	

				CMaterial* l_Material = m_Materials[l_CurrentSpriteMap->materialIndex];
				l_Material->Apply();

				m_VertexBuffers[l_CurrentSpriteMap->materialIndex]->UpdateVertexs(m_CurrentBufferData, l_CurrentVertex);
				m_VertexBuffers[l_CurrentSpriteMap->materialIndex]->Render(RenderManager, l_Material->GetRenderableObjectTechnique()->GetEffectTechnique(), &CEffectManager::m_SceneEffectParameters, l_CurrentVertex);
			}

			l_CurrentVertex = 0;
			l_CurrentSpriteMap = l_CommandSpriteMap;
		}
		

		float x1 = (l_Command.x1 / (m_ScreenWidth * 0.5f)) - 1.0f;
		float x2 = (l_Command.x2 / (m_ScreenWidth * 0.5f)) - 1.0f;
		float y1 = 1.0f - (l_Command.y1 / (m_ScreenHeight * 0.5f));
		float y2 = 1.0f - (l_Command.y2 / (m_ScreenHeight * 0.5f));

		float u1 = l_CommandSprite->u1 * (1.0f - l_Command.u1) + l_CommandSprite->u2 * l_Command.u1;
		float u2 = l_CommandSprite->u1 * (1.0f - l_Command.u2) + l_CommandSprite->u2 * l_Command.u2;
		float v1 = l_CommandSprite->v1 * (1.0f - l_Command.v1) + l_CommandSprite->v2 * l_Command.v1;
		float v2 = l_CommandSprite->v1 * (1.0f - l_Command.v2) + l_CommandSprite->v2 * l_Command.v2;

		m_CurrentBufferData[l_CurrentVertex++] = { Vect4f(x1, y2, 0.f, 1.f), l_Command.color, Vect2f(u1, v2) };
		m_CurrentBufferData[l_CurrentVertex++] = { Vect4f(x2, y2, 0.f, 1.f), l_Command.color, Vect2f(u2, v2) };
		m_CurrentBufferData[l_CurrentVertex++] = { Vect4f(x1, y1, 0.f, 1.f), l_Command.color, Vect2f(u1, v1) };
							
		m_CurrentBufferData[l_CurrentVertex++] = { Vect4f(x1, y1, 0.f, 1.f), l_Command.color, Vect2f(u1, v1) };
		m_CurrentBufferData[l_CurrentVertex++] = { Vect4f(x2, y2, 0.f, 1.f), l_Command.color, Vect2f(u2, v2) };
		m_CurrentBufferData[l_CurrentVertex++] = { Vect4f(x2, y1, 0.f, 1.f), l_Command.color, Vect2f(u2, v1) };
	}

	if (l_CurrentVertex > 0)
	{
		CMaterial* l_Material = m_Materials[l_CurrentSpriteMap->materialIndex];
		l_Material->Apply();

		m_VertexBuffers[l_CurrentSpriteMap->materialIndex]->UpdateVertexs(m_CurrentBufferData, l_CurrentVertex);
		m_VertexBuffers[l_CurrentSpriteMap->materialIndex]->Render(RenderManager, l_Material->GetRenderableObjectTechnique()->GetEffectTechnique(), &CEffectManager::m_SceneEffectParameters, l_CurrentVertex);
	}

	m_Commands.clear();
	m_CommandsExecutionOrder.clear();
}

CGUIManager::SFontChar::SFontChar(uint16 X, uint16 Y, uint16 Width, uint16 Height, int16 XOffset, int16 YOffset, int16 XAdvance, uint8 Page, uint8 Chnl)
{
	x = X;
	y = Y;
	width = Width;
	height = Height;
	xoffset = XOffset;
	yoffset = YOffset;
	xadvance = XAdvance;
	page = Page;
	chnl = Chnl;
}

void CGUIManager::AddFont(const std::string& FontName, const std::string& FontPath)
{
	CXMLTreeNode l_XML;

	if (l_XML.LoadFile(FontPath.c_str()))
	{
		std::unordered_map<wchar_t, SFontChar> &l_CharacterMap = m_CharactersPerFont[FontName];
		std::unordered_map<wchar_t, std::unordered_map<wchar_t, int>> l_Kernings = m_KerningPerFont[FontName];
		std::vector<SSpriteInfo*> &l_TextureArray = m_TexturePerFont[FontName];

		CXMLTreeNode l_Font = l_XML["font"];
		int l_W = 0;
		int l_H = 0;

		if (l_Font.Exists())
		{
			for (int i = 0; i < l_Font.GetNumChildren(); ++i)
			{
				CXMLTreeNode l_Element = l_Font(i);

				if (l_Element.GetName() == std::string("common"))
				{
					l_W = l_Element.GetIntProperty("scaleW");
					l_H = l_Element.GetIntProperty("scaleH");
					int l_LineHeight = l_Element.GetIntProperty("lineHeight");
					int l_Base = l_Element.GetIntProperty("base");
					m_LineHeightPerFont.insert(std::pair<std::string, int16>(FontName, l_LineHeight));
					m_BasePerFont.insert(std::pair<std::string, int16>(FontName, l_Base));
				}
				else if (l_Element.GetName() == std::string("pages"))
				{
					for (int i = 0; i < l_Element.GetNumChildren(); ++i)
					{
						CXMLTreeNode l_Page = l_Element(i);

						if (l_Page.GetName() == std::string("page"))
						{
							std::string l_TextureName = l_Page.GetPszProperty("file");
							
							assert(m_Sprites.find(l_TextureName) != m_Sprites.end());
							assert(m_Sprites[l_TextureName]->spriteMap->w = l_W);
							assert(m_Sprites[l_TextureName]->spriteMap->h = l_H);

							l_TextureArray.push_back(m_Sprites[l_TextureName]);
						}
					}
				}
				else if (l_Element.GetName() == std::string("chars"))
				{
					for (int i = 0; i < l_Element.GetNumChildren(); ++i)
					{
						CXMLTreeNode l_Char = l_Element(i);
						
						if (l_Char.GetName() == std::string("char"))
						{
							int l_ID = l_Char.GetIntProperty("id");
							int l_X = l_Char.GetIntProperty("x");
							int l_Y = l_Char.GetIntProperty("y");
							int l_Width = l_Char.GetIntProperty("width");
							int l_Height = l_Char.GetIntProperty("height");

							int l_Xoffset = l_Char.GetIntProperty("xoffset");
							int l_Yoffset = l_Char.GetIntProperty("yoffset");
							int l_Xadvance = l_Char.GetIntProperty("xadvance");

							int l_Page = l_Char.GetIntProperty("page");
							int l_Chnl = l_Char.GetIntProperty("chnl");

							SFontChar l_FontChar(l_X,l_Y,l_Width,l_Height,l_Xoffset,l_Yoffset,l_Xadvance,l_Page,l_Chnl);
						
							l_CharacterMap.insert(std::pair<wchar_t, CGUIManager::SFontChar>(l_ID, l_FontChar));
						}
					}
				}
				else if (l_Element.GetName() == std::string("kernings"))
				{
					for (int i = 0; i < l_Element.GetNumChildren(); ++i)
					{
						CXMLTreeNode l_Kerning = l_Element(i);

						if (l_Kerning.GetName() == std::string("kerning"))
						{
							wchar_t l_First = l_Element.GetIntProperty("first");
							wchar_t l_Second = l_Element.GetIntProperty("second");
							int l_Amount = l_Element.GetIntProperty("amount");

							std::unordered_map<wchar_t, int> l_UM;
							l_UM.insert(std::pair<wchar_t, int>(l_Second, l_Amount));

							l_Kernings.insert(std::pair<wchar_t, std::unordered_map<wchar_t, int>>(l_First, l_UM));
						}
					}
				}
			}
		}
	}
}

int CGUIManager::FillCommandQueueWithText(const std::string& Font, const std::string& Text, const CColor& Color, Vect4f *TextBox)
{
	Vect4f l_Dummy;

	if (TextBox == nullptr) 
		TextBox = &l_Dummy;

	*TextBox = Vect4f(0, 0, 0, 0);

	assert(m_LineHeightPerFont.find(Font) != m_LineHeightPerFont.end());
	assert(m_BasePerFont.find(Font) != m_BasePerFont.end());
	assert(m_CharactersPerFont.find(Font) != m_CharactersPerFont.end());
	assert(m_KerningPerFont.find(Font) != m_KerningPerFont.end());
	assert(m_TexturePerFont.find(Font) != m_TexturePerFont.end());

	int l_LineHeight = m_LineHeightPerFont[Font];
	int l_Baes = m_BasePerFont[Font];
	const std::unordered_map<wchar_t, SFontChar> &l_CharacterMap = m_CharactersPerFont[Font];
	const std::unordered_map<wchar_t, std::unordered_map<wchar_t, int>> l_Kernings = m_KerningPerFont[Font];
	const std::vector<SSpriteInfo*> &l_TextureArray = m_TexturePerFont[Font];

	wchar_t l_Last = 0;

	int l_CursorX = 0, l_CursorY = 0;

	float l_SpriteWidth = (float)l_TextureArray[0]->spriteMap->w;
	float l_SpriteHeight = (float)l_TextureArray[0]->spriteMap->h;

	int l_AddedCommands = 0;

	for (char c : Text)
	{
		if (c == '\n')
		{
			l_CursorY += l_LineHeight;
			l_CursorX = 0;
			l_Last = 0;
		}
		else
		{
			auto it = l_CharacterMap.find((wchar_t)c);
			if (it != l_CharacterMap.end())
			{
				const SFontChar &l_FontChar = it->second;

				auto it1 = l_Kernings.find(l_Last);
				if (it1 != l_Kernings.end())
				{
					auto it2 = it1->second.find(c);
					if (it2 != it1->second.end())
					{
						int l_Kerning = it2->second;
						l_CursorX += l_Kerning;
					}
				}

				SGUICommand l_Command = {};
				l_Command.sprite = l_TextureArray[l_FontChar.page];

				l_Command.x1 = l_CursorX + l_FontChar.xoffset;
				l_Command.x2 = l_Command.x1 + l_FontChar.width;
				l_Command.y1 = l_CursorY + l_FontChar.yoffset;
				l_Command.y2 = l_Command.y1 + l_FontChar.height;

				l_Command.u1 = (float)l_FontChar.x / l_SpriteWidth;
				l_Command.u2 = (float)(l_FontChar.x + l_FontChar.width) / l_SpriteWidth;

				l_Command.v1 = (float)l_FontChar.y / l_SpriteHeight;
				l_Command.v2 = (float)(l_FontChar.y + l_FontChar.height) / l_SpriteHeight;

				l_Command.color = Color;

				m_Commands.push_back(l_Command);
				++l_AddedCommands;
				l_Last = c;
				l_CursorX += l_FontChar.xadvance;

				if (l_Command.x1 < TextBox->x) TextBox->x = (float)l_Command.x1;
				if (l_Command.y1 < TextBox->y) TextBox->y = (float)l_Command.y1;
				if (l_Command.x2 < TextBox->z) TextBox->z = (float)l_Command.x2;
				if (l_Command.y2 < TextBox->w) TextBox->w = (float)l_Command.y2;
			}
			else
			{
				//Print unicode error char
			}
		}
	}

	return l_AddedCommands;
}

void CGUIManager::FillCommandQueueWithText(const std::string& Font, const std::string& Text, Vect2f Coord, CGUIManager::GUIAnchor Anchor, const CColor& Color)
{
	Vect4f l_TextSizes;
	
	int l_NumCommands = FillCommandQueueWithText(Font, Text, Color, &l_TextSizes);

	Vect2f l_Adjustment = Coord;

	if ((int)Anchor && (int)GUIAnchor::LEFT)
	{
		l_Adjustment.x -= l_TextSizes.x;
	}
	else if ((int)Anchor && (int)GUIAnchor::CENTER)
	{
		l_Adjustment.x -= (l_TextSizes.x + l_TextSizes.z) * 0.5f;
	}
	else if ((int)Anchor && (int)GUIAnchor::RIGHT)
	{
		l_Adjustment.x -= l_TextSizes.z;
	}
	else
	{
		assert(false);
	}

	if ((int)Anchor && (int)GUIAnchor::TOP)
	{
		l_Adjustment.y -= l_TextSizes.y;
	}
	else if ((int)Anchor && (int)GUIAnchor::MID)
	{
		l_Adjustment.y -= (l_TextSizes.y + l_TextSizes.w) * 0.5f;
	}
	else if ((int)Anchor && (int)GUIAnchor::BOTTOM)
	{
		l_Adjustment.y -= l_TextSizes.w;
	}
	
	else if ((int)Anchor && (int)GUIAnchor::BASE)
	{
		//
	}
	else
	{
		assert(false);
	}

	for (unsigned int i = m_Commands.size() - l_NumCommands; i < m_Commands.size(); ++i)
	{
		m_Commands[i].x1 += (int)l_Adjustment.x;
		m_Commands[i].x2 += (int)l_Adjustment.x;
		m_Commands[i].y1 += (int)l_Adjustment.y;
		m_Commands[i].y2 += (int)l_Adjustment.y;
	}
}