#include "TTODFileUtils.h"
#include <commdlg.h>
#include "Engine.h"
#include "Input\InputManagerImplementation.h"
//#include "Utils\EmptyPointerClass.h"

std::string CTTODFileUtils::SelectTextureFile(HWND Hwnd, const std::string  &InFile)
{
	OPENFILENAME of;
	//char szFileName[100] = "";

	ZeroMemory(&of, sizeof(of));

	of.lStructSize = sizeof(of);
	of.hwndOwner = CEngine::GetSingleton().GetInputManager()->GetHwnd();
	//of.lpstrFilter = "Text Files (*.txt)\0*.txt\0";
	of.lpstrFilter = "PNG Files(.png)\0*.png\0DDS Files(.dds)\0*.dds\0All Files(*.*)\0*.*\0"; //"PNG Files(.png)\0*.ASM\0DDS Files(.dds)\0*.dds\0All Files(*.*)\0*.*\0";
	char l_File[MAX_PATH];
	strcpy_s(l_File, MAX_PATH, InFile.c_str());
	of.lpstrFile = l_File;
	of.nMaxFile = MAX_PATH;
	of.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	of.lpstrDefExt = "dds";

	if (GetOpenFileName(&of))
	{
		return std::string(of.lpstrFile);
	}
	return "";
}