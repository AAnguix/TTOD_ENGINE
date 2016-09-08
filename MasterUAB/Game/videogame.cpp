#include "Game.h"
#include <vld.h> //Visual Leak Detector
#include <memory>

void CheckMemoryLeaks()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(282314); 
}

int APIENTRY WinMain(HINSTANCE _hInstance, HINSTANCE _hPrevInstance, LPSTR _lpCmdLine, int _nCmdShow)
{
	CheckMemoryLeaks();

	bool l_Result;
	CGame* l_Game = new CGame;
	if (!l_Game){ return 0; }

	l_Result = l_Game->Initialize();
	if (l_Result){ l_Game->Run(); }

	l_Game->Shutdown();
	delete l_Game;
	l_Game = nullptr;

	return 0;
}
