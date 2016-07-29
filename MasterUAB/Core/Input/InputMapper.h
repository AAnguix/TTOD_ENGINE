#ifndef _INPUTMAPPER_H
#define _INPUTMAPPER_H

#include "Math\Vector2.h"

class CInputMapper
{

protected:
	CInputMapper();
	Vect2i m_DirectXCursor;

	bool m_Focus;

public:
	
	Vect2i GetDirectXCursor() const { return m_DirectXCursor; }
	bool HasFocus() const { return m_Focus; }

	static CInputMapper* GetInputMapper();
	static void SetCurrentInputMapper(CInputMapper* InputMapper);
};

#endif
