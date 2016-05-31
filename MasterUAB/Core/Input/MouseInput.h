#ifndef _MOUSEINPUT_H
#define _MOUSEINPUT_H

class CMouseInput
{
private:
	long m_X;
	long m_Y;

	bool m_LeftButtonBecomesPressed;
	bool m_LeftButtonBecomesReleased;

public:
	CMouseInput();
	virtual ~CMouseInput();
	long GetX() const{ return m_X; };
	long GetY() const{ return m_Y; };
	void Update(long X, long Y, bool LeftWentPressed, bool LeftWentReleased);
	bool LeftButtonBecomesPressed() const { return m_LeftButtonBecomesPressed; };
	bool LeftButtonBecomesReleased() const { return m_LeftButtonBecomesReleased; };
};

#endif