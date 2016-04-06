#include "Input\KeyBoardInput.h"

CKeyBoardInput::CKeyBoardInput()
{

}

CKeyBoardInput::~CKeyBoardInput()
{

}

wchar_t CKeyBoardInput::ConsumeLastChar()
{
	wchar_t l_Char = 0x00;
	if (!m_Chars.empty())
	{
		l_Char = m_Chars.front();
		m_Chars.pop();
	}
	
	return l_Char;
}

void CKeyBoardInput::SetLastChar(wchar_t Char)
{
	m_Chars.push(Char);
}

