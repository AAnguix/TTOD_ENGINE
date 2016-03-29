#ifndef _MOUSEINPUT_H
#define _MOUSEINPUT_H

class CMouseInput
{
private:
	size_t m_X;
	size_t m_Y;

	bool m_LeftButtonBecomesPressed;
	bool m_LeftButtonBecomesReleased;

public:
	CMouseInput();
	virtual ~CMouseInput();
	size_t GetX() const{ return m_X; };
	size_t GetY() const{ return m_Y; };
	void Update(size_t X, size_t Y, bool LeftWentPressed, bool LeftWentReleased);
	bool LeftButtonBecomesPressed() const { return m_LeftButtonBecomesPressed; };
	bool LeftButtonBecomesReleased() const { return m_LeftButtonBecomesReleased; };
};

#endif