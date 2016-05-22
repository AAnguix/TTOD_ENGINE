#ifndef _TTODFILEUTILS_H
#define _TTODFILEUTILS_H

#include <string>
#include <windows.h>

class CTTODFileUtils
{

public:
	 static std::string SelectTextureFile(HWND Hwnd, const std::string  &InFile);
};

#endif