#ifndef _MOUSEINPUT_H
#define _MOUSEINPUT_H

class CMouseInput
{
private:
	float m_X;
	float m_Y;

	bool m_LeftButtonBecomesPressed;
	bool m_LeftButtonBecomesReleased;

public:
	CMouseInput();
	virtual ~CMouseInput();
	float GetX() const{ return m_X; };
	float GetY() const{ return m_Y; };
	void Update(float X, float Y, bool LeftWentPressed, bool LeftWentReleased);
	bool LeftButtonBecomesPressed() const { return m_LeftButtonBecomesPressed; };
	bool LeftButtonBecomesReleased() const { return m_LeftButtonBecomesReleased; };
};

#endif