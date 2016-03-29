#ifndef _GUIMANAGER_H
#define _GUIMANAGER_H

#include <string>
#include <vector>
#include <map>
#include "Math\Color.h"
class CMaterial;
class CRenderableVertexs;
class CRenderManager;
class CXMLTreeNode;
#include "Vertex\RenderableVertexs.h"
#include "Vertex\VertexTypes.h"
const unsigned int s_MaxVerticesPerCall = 60;
const unsigned int s_unitPixelSizeX = 1;
const unsigned int s_unitPixelSizeY = 1;
class CGUIManager 
{ 
public:
	enum class GUICoordType
	{
		GUI_ABSOLUTE,
		GUI_RELATIVE,
		GUI_RELATIVE_WIDTH,
		GUI_RELATIVE_HEIGHT
	};
	enum class GUIAnchor
	{
		TOP = 0x1,
		MID = 0x2,
		BOTTOM = 0x4,

		LEFT = 0x10,
		CENTER = 0x20,
		RIGHT = 0x40,

		TOP_LEFT = TOP | LEFT,
		TOP_CENTER = TOP | CENTER,
		TOP_RIGHT = TOP | RIGHT,
		MID_LEFT = MID | LEFT,
		MID_CENTER = MID | CENTER,
		MID_RIGHT = MID | RIGHT,
		BOTTOM_LEFT = BOTTOM | LEFT,
		BOTTOM_CENTER = BOTTOM | CENTER,
		BOTTOM_RIGHT = BOTTOM | RIGHT,
	};

public:
	struct SGUIPosition
	{
		float x;
		float y;
		float width;
		float height;
		SGUIPosition(float X, float Y, float Width, float Height, 
		GUIAnchor Anchor = GUIAnchor::TOP_LEFT, GUICoordType AnchorCoordsType = GUICoordType::GUI_ABSOLUTE, GUICoordType SizeCoordsType = GUICoordType::GUI_ABSOLUTE)
		: x(X), y(Y), width(Width), height(Height)
		{
			/*switch (SizeCoordsType)
			{
				default:
					assert(false);
				case GUICoordType::GUI_ABSOLUTE:
					width = Width;
					height = Height;
					break;
				case GUICoordType::GUI_RELATIVE:
					width = (int) Width * m_ScreenWidth;
					height = (int) Height * m_ScreenHeight;
					break;
				case GUICoordType::GUI_RELATIVE_WIDTH:
					width = (int) Width * w;
					height = (int) Height * w;
					break;
				case GUICoordType::GUI_RELATIVE_HEIGHT:
					width = (int)Width * h;
					height = (int)Height * h;
					break;
			}

			if (x < 0){x = 1.0f + x;}
			if (y < 0){y = 1.0f + y;}*/

			/*if ((int)Anchor && (int)GUIAnchor::LEFT)
			{
				x = (int)(x * s_unitPixelSizeX);
			}
			else if ((int)Anchor && (int)GUIAnchor::LEFT)
			{
				x = (int)((x * s_unitPixelSizeX) - (width*0.5f));
			}
			else if ((int)Anchor && (int)GUIAnchor::LEFT)
			{
				x = (int)(x * s_unitPixelSizeX - width);
			}
			else { assert(false); }

			if ((int)Anchor && (int)GUIAnchor::TOP)
			{
				y = (int)(y * s_unitPixelSizeY);
			}
			else if ((int)Anchor && (int)GUIAnchor::MID)
			{
				y = (int)((y * s_unitPixelSizeY) - (height*0.5f));
			}
			else if ((int)Anchor && (int)GUIAnchor::BOTTOM)
			{
				y = (int)(y * s_unitPixelSizeY - height);
			}
			else { assert(false); }*/
		};
	};
	struct SSliderResult
	{
		float real;
		float temp;
	};

private:
	struct SSpriteMapInfo
	{
		std::string name;
		int materialIndex;
		int w, h;
		SSpriteMapInfo(const std::string& Name, int MaterialIndex, int W, int H) : materialIndex(MaterialIndex), w(W), h(H){};
	};
	struct SSpriteInfo
	{
		SSpriteMapInfo *spriteMap;
		float u1, u2, v1, v2;
		SSpriteInfo(SSpriteMapInfo* SpriteMap, float U1, float U2, float V1, float V2) : spriteMap(SpriteMap), u1(U1), u2(U2), v1(V1), v2(V2){};
	};
	struct SGUICommand
	{
		SSpriteInfo *sprite;
		int x1, y1, x2, y2;
		float u1, v1, u2, v2;
		CColor color;
	};
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
	struct SButtonInfo
	{
		SSpriteInfo* normal;
		SSpriteInfo* highlight;
		SSpriteInfo* pressed;
		SButtonInfo(SSpriteInfo* Normal, SSpriteInfo* HighLight, SSpriteInfo* Pressed) : normal(Normal), highlight(HighLight), pressed(Pressed){};
	};

	MV_POSITION4_COLOR_TEXTURE_VERTEX m_CurrentBufferData[s_MaxVerticesPerCall];
	int m_CurrentVertex;
	SSpriteMapInfo* m_CurrentSpriteMap;

	unsigned int m_ScreenWidth;
	unsigned int m_ScreenHeight;

	std::string m_Filename; 
	std::string m_ActiveItem;
	std::string m_HotItem;

	size_t m_MouseX;
	size_t m_MouseY;
	bool m_InputUpToDate;

	bool m_MouseWentPressed;
	bool m_MouseWentReleased;

	std::vector<CRenderableVertexs*> m_VertexBuffers;
	std::vector<CMaterial*> m_Materials;
	std::vector<SGUICommand> m_Commands;
	std::vector<size_t> m_CommandsExecutionOrder;

	std::map<std::string,SSpriteMapInfo*> m_SpriteMaps;
	std::map<std::string, SSpriteInfo*> m_Sprites;
	std::map<std::string, SSliderInfo*> m_Sliders;
	std::map<std::string, SButtonInfo*> m_Buttons;
	
	void AddSpriteMap(CXMLTreeNode &TreeNode);
	SSpriteInfo* GetSprite(const std::string& Name);
	SSliderInfo* GetSlider(const std::string& SliderID);
	SButtonInfo* GetButton(const std::string& ButtonID);

	bool IsMouseInside(unsigned int MouseX, unsigned int MouseY, unsigned int X, unsigned int Y, unsigned int Width, unsigned int Height); 
	void SortCommands();
	void CheckInput();

	void SetActive(const std::string& ID);
	void SetNotActive(const std::string& ID);
	void SetHot(const std::string& ID);
	void SetNotHot(const std::string& ID);

	void Destroy();

public: 
	CGUIManager();
	virtual ~CGUIManager();
	void Initialize(unsigned int ScreenWidth, unsigned int ScreenHeight);
	void Load(const std::string &Filename);
	void Reload();
	void Render(CRenderManager* RenderManager);

	void AddButton(const std::string& ButtonID, const std::string& Normal, const std::string& Highlight, const std::string& Pressed);
	bool DoButton(const std::string& GuiID, const std::string& ButtonID, const SGUIPosition& Position);

	void AddSlider(const std::string& SliderID, const std::string& Base, const std::string& Top, const std::string& Handle, const std::string& PressedHandle);
	SSliderResult DoSlider(const std::string& GuiID, const std::string& SliderID, const SGUIPosition& Position, float MinValue, float MaxValue, float CurrentValue);
	
};

#endif