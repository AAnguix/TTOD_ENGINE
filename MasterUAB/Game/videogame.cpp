#include "Game.h"
#include <vld.h> //Visual Leak Detector

void CheckMemoryLeaks()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(21892); 
}

int APIENTRY WinMain(HINSTANCE _hInstance, HINSTANCE _hPrevInstance, LPSTR _lpCmdLine, int _nCmdShow)
{
	CheckMemoryLeaks();

	CGame* l_Game;
	bool l_Result;

	l_Game = new CGame;
	if (!l_Game){ return 0; }

	l_Result = l_Game->Initialize();
	if (l_Result){ l_Game->Run(); }

	l_Game->Shutdown();
	delete l_Game;
	l_Game = 0;

	return 0;
}
