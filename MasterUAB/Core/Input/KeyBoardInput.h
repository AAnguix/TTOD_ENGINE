#ifndef _KEYBOARDINPUT_H
#define _KEYBOARDINPUT_H

#include <queue>

class CKeyBoardInput
{

private:
	std::queue<wchar_t> m_Chars;

public:
	CKeyBoardInput();
	virtual ~CKeyBoardInput();

	void SetLastChar(wchar_t Char);
	wchar_t ConsumeLastChar();
};

#endif