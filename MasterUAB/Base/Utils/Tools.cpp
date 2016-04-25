#include "Tools.h"

wchar_t* CTools::CharToWChar(const char* Text)
{
	/*size_t size = strlen(Text) + 1;
	wchar_t* wa = new wchar_t[size];
	mbstowcs(wa, Text, size);
	return wa;*/
	size_t size = strlen(Text) + 1;
	wchar_t* portName = new wchar_t[size];
	//wchar_t* portName = new wchar_t[size];
	size_t outSize;
	mbstowcs_s(&outSize, portName, size, Text, size - 1);
	return portName;
}