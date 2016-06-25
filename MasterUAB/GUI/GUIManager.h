#ifndef _GUIMANAGER_H
#define _GUIMANAGER_H

#include <string>
#include <vector>
#include <unordered_map>
#include <map>
#include "Math\Color.h"
class CMaterial;
class CRenderableVertexs;
class CRenderManager;
class CXMLTreeNode;
#include "Vertex\RenderableVertexs.h"
#include "Vertex\VertexTypes.h"
const unsigned int s_MaxVerticesPerCall = 240;

class CGUIManager
{
public:
	enum GUICoordType
	{
		GUI_ABSOLUTE,
		GUI_RELATIVE,
		GUI_RELATIVE_WIDTH,
		GUI_RELATIVE_HEIGHT
	};
	enum GUIAnchor
	{
		TOP = 0x1,
		MID = 0x2,
		BOTTOM = 0x4,

		LEFT = 0x10,
		CENTER = 0x20,
		RIGHT = 0x40,

		BASE = 0x80,

		TOP_LEFT = TOP | LEFT,
		TOP_CENTER = TOP | CENTER,
		TOP_RIGHT = TOP | RIGHT,
		MID_LEFT = MID | LEFT,
		MID_CENTER = MID | CENTER,
		MID_RIGHT = MID | RIGHT,
		BOTTOM_LEFT = BOTTOM | LEFT,
		BOTTOM_CENTER = BOTTOM | CENTER,
		BOTTOM_RIGHT = BOTTOM | RIGHT,
		BASE_LEFT = BASE | LEFT,
		BASE_CENTER = BASE | CENTER,
		BASE_RIGHT = BASE | RIGHT
	};

public:
	struct SGUIPosition
	{
		float x;
		float y;
		float width;
		float height;
		SGUIPosition(float X, float Y, float Width, float Height, GUIAnchor Anchor, GUICoordType AnchorCoordsType, GUICoordType SizeCoordsType);
		SGUIPosition(float X, float Y, float Width, float Height);
	};
	struct SSliderResult
	{
		float real;
		float temp;
		SSliderResult():real(0.0f), temp(0.0f){};
	};
	struct SHealthBarResult
	{
		float value;
		SHealthBarResult(){};
	};

	struct SSpriteMapInfo
	{
		std::string name;
		int materialIndex;
		int w, h;
		SSpriteMapInfo(const std::string& Name, int MaterialIndex, int W, int H) : materialIndex(MaterialIndex), w(W), h(H), name(Name){};
		virtual ~SSpriteMapInfo(){};
	};

	struct SSpriteInfo
	{
		SSpriteMapInfo *spriteMap;
		float u1, u2, v1, v2;
		SSpriteInfo(SSpriteMapInfo* SpriteMap, float U1, float U2, float V1, float V2) : spriteMap(SpriteMap), u1(U1), u2(U2), v1(V1), v2(V2){};
		virtual ~SSpriteInfo(){};
	};

private:
	struct SGUICommand
	{
		SSpriteInfo *sprite;
		int x1, y1, x2, y2;
		float u1, v1, u2, v2;
		CColor color;
	};
	
	/*GUI Elements*/
	struct SSliderInfo
	{
		SSpriteInfo* base;
		SSpriteInfo* top;
		SSpriteInfo* handle;
		SSpriteInfo* pressed_handle;
		float handleRelativeWidth;
		float handleRelativeHeight;
		SSliderInfo(SSpriteInfo* Base, SSpriteInfo* Top, SSpriteInfo* Handle, SSpriteInfo* Pressed_Handle) : base(Base), top(Top), handle(Handle), pressed_handle(Pressed_Handle)
		{
			float x1 = Handle->u1 * Handle->spriteMap->w;
			float x2 = Handle->u2 * Handle->spriteMap->w;
			float y1 = Handle->v1 * Handle->spriteMap->h;
			float y2 = Handle->v2 * Handle->spriteMap->h;

			handleRelativeWidth = (x2 - x1) / Handle->spriteMap->w;
			handleRelativeHeight = (y2 - y1) / Handle->spriteMap->h;
		};
	};
	struct SHealthBarInfo
	{
		SSpriteInfo* base;
		SSpriteInfo* top;
		SSpriteInfo* background;
		float barRelativeWidth;
		float barRelativeHeight;
		SHealthBarInfo(SSpriteInfo* Base, SSpriteInfo* Top, SSpriteInfo* Background) : base(Base), top(Top), background(Background)
		{
			float x1 = Background->u1 * Background->spriteMap->w;
			float x2 = Background->u2 * Background->spriteMap->w;
			float y1 = Background->v1 * Background->spriteMap->h;
			float y2 = Background->v2 * Background->spriteMap->h;

			barRelativeWidth = (x2 - x1) / Background->spriteMap->w;
			barRelativeHeight = (y2 - y1) / Background->spriteMap->h;
		};
	};
	struct SButtonInfo
	{
		SSpriteInfo* normal;
		SSpriteInfo* highlight;
		SSpriteInfo* pressed;
		SButtonInfo(SSpriteInfo* Normal, SSpriteInfo* HighLight, SSpriteInfo* Pressed) : normal(Normal), highlight(HighLight), pressed(Pressed){};
	};
	struct SImageInfo
	{
		SSpriteInfo* sprite;
		SImageInfo(SSpriteInfo* Sprite) : sprite(Sprite){};
	};

private:
	struct SFontChar
	{
		uint16 x, y, width, height;
		int16 xoffset, yoffset, xadvance;
		uint8 page, chnl;
		SFontChar();
		SFontChar(uint16 X, uint16 Y, uint16 Width, uint16 Height, int16 XOffset, int16 YOffset, int16 XAdvance, uint8 Page, uint8 Chnl);
	};
	std::unordered_map<std::string, int16> m_LineHeightPerFont;
	std::unordered_map<std::string, int16> m_BasePerFont;
	std::unordered_map<std::string, std::unordered_map<wchar_t, SFontChar>> m_CharactersPerFont;
	std::unordered_map<std::string, std::unordered_map<wchar_t, std::unordered_map<wchar_t, int> >> m_KerningPerFont;
	std::unordered_map<std::string, std::vector<SSpriteInfo*>> m_TexturePerFont;
	float m_AnimationTimer;
	
	int FillCommandQueueWithText(const std::string& Font, const std::string& Text, const CColor& Color = CColor(1,1,1,1), Vect4f *TextBox = nullptr);
	void FillCommandQueueWithText(const std::string& Font, const std::string& Text, Vect2f Coord, GUIAnchor Anchor = GUIAnchor::BASE_LEFT, const CColor& Color = CColor(1, 1, 1, 1));
	
	MV_POSITION4_COLOR_TEXTURE_VERTEX m_CurrentBufferData[s_MaxVerticesPerCall];

	float m_ScreenWidth;
	float m_ScreenHeight;

	std::vector<std::string> m_Filenames;

	std::string m_ActiveItem;
	std::string m_HotItem;
	std::string m_SelectedItem;

	float m_MouseX;
	float m_MouseY;
	bool m_InputUpToDate;

	bool m_MouseWentPressed;
	bool m_MouseWentReleased;

	std::vector<CRenderableVertexs*> m_VertexBuffers;
	std::vector<CMaterial*> m_Materials;
	std::vector<SGUICommand> m_Commands;
	std::vector<unsigned int> m_CommandsExecutionOrder;

	std::map<std::string, SSpriteMapInfo*> m_SpriteMaps;
	std::map<std::string, SSpriteInfo*> m_Sprites;
	std::map<std::string, SSliderInfo*> m_Sliders;
	std::map<std::string, SHealthBarInfo*> m_HealthBars;
	std::map<std::string, SButtonInfo*> m_Buttons;
	std::map<std::string, SImageInfo*> m_Images;

	void AddSpriteMap(CXMLTreeNode &TreeNode);
	SSpriteInfo* GetSprite(const std::string& Name);
	SSliderInfo* GetSlider(const std::string& SliderID);
	SHealthBarInfo* GetHealthBar(const std::string& HealthBarID);
	SButtonInfo* GetButton(const std::string& ButtonID);
	SImageInfo* GetImage(const std::string& ImageID);

	bool IsMouseInside(float MouseX, float MouseY, float X, float Y, float Width, float Height);
	void SortCommands();
	void CheckInput();

	void SetActive(const std::string& ID);
	void SetNotActive(const std::string& ID);
	void SetHot(const std::string& ID);
	void SetNotHot(const std::string& ID);
	void SetSelected(const std::string& ID);
	void SetNotSelected(const std::string& ID);

	

public:
	CGUIManager();
	virtual ~CGUIManager();
	void Destroy();

	float GetScreenWidth(){ return m_ScreenWidth; };
	float GetScreenHeight(){ return m_ScreenHeight; };

	void Initialize(float ScreenWidth, float ScreenHeight);
	void Load(const std::string &Filename);
	void Reload();
	void Render(CRenderManager* RenderManager);

	void AddImage(const std::string& ImageID, const std::string& Sprite);
	void AddButton(const std::string& ButtonID, const std::string& Normal, const std::string& Highlight, const std::string& Pressed);
	void AddSlider(const std::string& SliderID, const std::string& Base, const std::string& Top, const std::string& Handle, const std::string& PressedHandle);
	void AddHealthBar(const std::string& HealthBarID, const std::string& Base, const std::string& Top, const std::string& Background);
	
	bool DoButton(const std::string& GuiID, const std::string& ButtonID, const SGUIPosition& Position);
	SSliderResult DoSlider(const std::string& GuiID, const std::string& SliderID, const SGUIPosition& Position, float MinValue, float MaxValue, float CurrentValue);
	void DoHealthBar(const std::string& GuiID, const std::string& HealthBarID, const SGUIPosition& Position, float MinValue, float MaxValue, float CurrentValue);
	void DoImage(const std::string& GuiID, const std::string& ImageID, const SGUIPosition& Position);

	void AddFont(const std::string& FontName, const std::string& FontPath);

	void DoText(const std::string& GuiID, const std::string& Font, const SGUIPosition& Position, const std::string& Sprite, const std::string& Text);
	std::string DoTextBox(const std::string& GuiID, const std::string& Font, const SGUIPosition& Position, const std::string& Sprite, const std::string& CurrentText, float ElapsedTime);
	void CreateConsole(const std::string& GuiID, const std::string& Font, const SGUIPosition& Position, const std::string& Sprite, float ElapsedTime);
};

#endif