#include "Input\MouseInput.h"

CMouseInput::CMouseInput()
{

}

CMouseInput::~CMouseInput()
{

}

void CMouseInput::Update(size_t X, size_t Y, bool LeftWentPressed, bool LeftWentReleased)
{ 
	m_X = X; 
	m_Y = Y; 
	m_LeftButtonBecomesPressed = LeftWentPressed; 
	m_LeftButtonBecomesReleased = LeftWentReleased; 
};