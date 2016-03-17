#ifndef _GUIMANAGER_H
#define _GUIMANAGER_H

#include <string>
#include <vector>
class CMaterial;
class CRenderableVertexs;
class CRenderManager;

class CGUIManager 
{ 

private: 
	std::string m_Filename; 
	//std::vector<CMaterial*> l_MaterialsVector;
	std::string m_ActiveItem;
	std::string m_HotItem;

	size_t m_MouseX;
	size_t m_MouseY;
	bool m_InputUpToDate;

	bool m_MouseWentPressed;
	bool m_MouseWentReleased;


	std::vector<CRenderableVertexs*> m_VertexBuffers;
	std::vector<CMaterial*> m_Materials;

public: 
	CGUIManager();
	virtual ~CGUIManager();

	struct SSpriteMapInfo
	{
		int materialIndex;
		int w, h;
		SSpriteMapInfo(int MaterialIndex, int W, int H) : materialIndex(MaterialIndex),w(W),h(H){};
	};
	struct SpriteInfo
	{
		SSpriteMapInfo *spriteMap;
		float u1, u2, v1, v2;
		SpriteInfo(SSpriteMapInfo* SpriteMap, float U1, float U2, float V1, float V2) : spriteMap(SpriteMap), u1(U1), u2(U2), v1(V1), v2(V2){};
	};
	struct SGUIPosition
	{
		size_t x;
		size_t y;
		size_t width;
		size_t height;
		SGUIPosition(size_t X, size_t Y, size_t Width, size_t Height) : x(X),y(Y),width(Width),height(Height){};
	};

	bool DoButton(const std::string& GuiID, const std::string& ButtonID, const SGUIPosition& Position);

	struct SSliderResult
	{
		float real;
		float temp;
	};

	SSliderResult DoSlider(const std::string& GuiID, const std::string& SliderID, const SGUIPosition& Position, float MinValue, float MaxValue, float CurrentValue);

	void SetActive(const std::string& ID);
	void SetNotActive(const std::string& ID);
	void SetHot(const std::string& ID);
	void SetNotHot(const std::string& ID);

	void CheckInput();

	void Load(const std::string &Filename);  
	void Reload(); 
	void Render(CRenderManager* RenderManager);
};

#endif