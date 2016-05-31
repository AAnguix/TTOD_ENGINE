#include "Input\MouseInput.h"

CMouseInput::CMouseInput()
{

}

CMouseInput::~CMouseInput()
{

}

void CMouseInput::Update(long X, long Y, bool LeftWentPressed, bool LeftWentReleased)
{ 
	m_X = X; 
	m_Y = Y; 
	m_LeftButtonBecomesPressed = LeftWentPressed; 
	m_LeftButtonBecomesReleased = LeftWentReleased; 
};