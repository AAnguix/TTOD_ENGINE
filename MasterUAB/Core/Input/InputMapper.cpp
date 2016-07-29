#include "Input\InputMapper.h"
static CInputMapper* s_CurrentInputMapper;

class CDummyInputMapper : public CInputMapper
{
public:
	CDummyInputMapper()
	{
		s_CurrentInputMapper = this;
	}
};

static CDummyInputMapper s_DummyInputManager;

CInputMapper::CInputMapper()
:m_DirectXCursor(0,0)
,m_Focus(false)
{

}

CInputMapper* CInputMapper::GetInputMapper()
{
	return s_CurrentInputMapper;
}

void CInputMapper::SetCurrentInputMapper(CInputMapper* InputMapper)
{
	if (InputMapper)
	{
		s_CurrentInputMapper = InputMapper;
	}
	else
	{
		s_CurrentInputMapper = &s_DummyInputManager;
	}
}
