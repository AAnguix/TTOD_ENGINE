#include "Input\InputContext.h"
#include "Input\FileReader.h"
#include <fstream>
#include "XML\XMLTreeNode.h"

#include "Input\InputMapperImplementation.h"

using namespace InputMapping;

//CInputContext::CInputContext(const std::wstring& FileName)
//:m_Conversions(NULL)
//{
//	unsigned l_Ok;
//	std::wifstream l_Infile(FileName);
//	if (!(l_Infile >> l_Ok)){ assert(false); }
//
//	unsigned l_Ranges = ReadValue<unsigned>(l_Infile);
//
//	for (unsigned i = 0; i < l_Ranges; ++i)
//	{
//		OSInputAxis axis = static_cast<OSInputAxis>(ReadValue<unsigned>(l_Infile));
//		ERange range = static_cast<ERange>(ReadValue<unsigned>(l_Infile));
//		m_RangeMap[axis] = range;
//	}
//
//	unsigned l_States = ReadValue<unsigned>(l_Infile);
//	for (unsigned i = 0; i < l_States; ++i)
//	{
//		OSInputButton l_Button = static_cast<OSInputButton>(ReadValue<unsigned>(l_Infile));
//		EState l_State = static_cast<EState>(ReadValue<unsigned>(l_Infile));
//		m_StateMap[l_Button] = l_State;
//	}
//
//	unsigned l_Actions = ReadValue<unsigned>(l_Infile);
//	for (unsigned i = 0; i < l_Actions; ++i)
//	{
//		OSInputButton l_Button = static_cast<OSInputButton>(ReadValue<unsigned>(l_Infile));
//		EAction l_Action = static_cast<EAction>(ReadValue<unsigned>(l_Infile));
//		m_ActionMap[l_Button] = l_Action;
//	}
//
//	m_Conversions = new CRangeConverter(l_Infile);
//
//	unsigned sensitivitycount = ReadValue<unsigned>(l_Infile);
//	for(unsigned i = 0; i < sensitivitycount; ++i)
//	{
//		ERange range = static_cast<ERange>(ReadValue<unsigned>(l_Infile));
//		double sensitivity = ReadValue<double>(l_Infile);
//		m_SensitivityMap[range] = sensitivity;
//	}
//}

CInputContext::CInputContext(const std::string &ContextFileName)
:m_Conversions(NULL)
{
	CXMLTreeNode l_XML;

	if (l_XML.LoadFile(ContextFileName.c_str()))
	{
		CXMLTreeNode l_Context = l_XML["input_context"];

		if (l_Context.Exists())
		{
			for (int i = 0; i < l_Context.GetNumChildren(); ++i)
			{
				CXMLTreeNode l_Element = l_Context(i);

				if (l_Element.GetName() == std::string("ranges"))
				{
					for (int i = 0; i < l_Element.GetNumChildren(); ++i)
					{
						if (l_Element(i).GetName() == std::string("range"))
						{
							EOSInputAxis l_Axis = static_cast<EOSInputAxis>(l_Element(i).GetIntProperty("axis", 0));
							ERange l_Range = static_cast<ERange>(l_Element(i).GetIntProperty("range", 0));
							m_RangeMap[l_Axis] = l_Range;
						}
					}
				}
				else if (l_Element.GetName() == std::string("states"))
				{
					for (int i = 0; i < l_Element.GetNumChildren(); ++i)
					{
						if (l_Element(i).GetName() == std::string("state"))
						{
							EOSInputButtonX l_Button = StringToOSButton(l_Element(i).GetPszProperty("button", ""));
							bool l_Alt = l_Element(i).GetBoolProperty("alt", false);
							bool l_Ctrl = l_Element(i).GetBoolProperty("ctrl", false);
							SOSInputButtons l_Buttons = SOSInputButtons(l_Button, l_Alt, l_Ctrl);

							EState l_State = static_cast<EState>(l_Element(i).GetIntProperty("state", 0));
							m_StateMap[l_Buttons] = l_State;
						}
					}
				}
				else if (l_Element.GetName() == std::string("actions"))
				{
					for (int i = 0; i < l_Element.GetNumChildren(); ++i)
					{
						if (l_Element(i).GetName() == std::string("action"))
						{
							EOSInputButtonX l_Button = StringToOSButton(l_Element(i).GetPszProperty("button", ""));
							bool l_Alt = l_Element(i).GetBoolProperty("alt", false);
							bool l_Ctrl = l_Element(i).GetBoolProperty("ctrl", false);
							SOSInputButtons l_Buttons = SOSInputButtons(l_Button, l_Alt, l_Ctrl);

							EAction l_Action = static_cast<EAction>(l_Element(i).GetIntProperty("action", 0));
							m_ActionMap[l_Buttons] = l_Action;
						}
					}
				}
				else if (l_Element.GetName() == std::string("conversions"))
				{
					m_Conversions = new CRangeConverter(l_Element);
				}
				else if (l_Element.GetName() == std::string("sensivities"))
				{
					for (int i = 0; i < l_Element.GetNumChildren(); ++i)
					{
						if (l_Element(i).GetName() == std::string("sensivity"))
						{
							ERange l_Range = static_cast<ERange>(l_Element(i).GetIntProperty("range", 0));
							m_SensitivityMap[l_Range] = l_Element(i).GetIntProperty("sensivity", 0);
						}
					}
				}
			}
		}
		else { assert(false); }
	}
	else { assert(false); }
}

InputMapping::EOSInputButtonX CInputContext::StringToOSButton(const std::string& Key)
{
	assert(Key != "");

	if (Key == "MOUSE_LEFT")
		return OS_INPUT_BUTTON_MOUSE_LEFT;
	else if (Key == "MOUSE_RIGHT")
		return OS_INPUT_BUTTON_MOUSE_RIGHT;
	else if (Key == "MOUSE_WHEEL")
		return OS_INPUT_BUTTON_MOUSE_WHEEL;
	else if (Key == "A")
		return OS_INPUT_BUTTON_A;
	else if (Key=="B")
		return OS_INPUT_BUTTON_B;
	else if (Key == "C")
		return OS_INPUT_BUTTON_C;
	else if (Key == "D")
		return OS_INPUT_BUTTON_D;
	else if (Key == "E")
		return OS_INPUT_BUTTON_E;
	else if (Key == "F")
		return OS_INPUT_BUTTON_F;
	else if (Key == "G")
		return OS_INPUT_BUTTON_G;
	else if (Key == "H")
		return OS_INPUT_BUTTON_H;
	else if (Key == "I")
		return OS_INPUT_BUTTON_I;
	else if (Key == "J")
		return OS_INPUT_BUTTON_J;
	else if (Key == "K")
		return OS_INPUT_BUTTON_K;
	else if (Key == "L")
		return OS_INPUT_BUTTON_L;
	else if (Key == "M")
		return OS_INPUT_BUTTON_M;
	else if (Key == "N")
		return OS_INPUT_BUTTON_N;
	else if (Key == "O")
		return OS_INPUT_BUTTON_O;
	else if (Key == "P")
		return OS_INPUT_BUTTON_P;
	else if (Key == "Q")
		return OS_INPUT_BUTTON_Q;
	else if (Key == "R")
		return OS_INPUT_BUTTON_R;
	else if (Key == "S")
		return OS_INPUT_BUTTON_S;
	else if (Key == "T")
		return OS_INPUT_BUTTON_T;
	else if (Key == "U")
		return OS_INPUT_BUTTON_U;
	else if (Key == "V")
		return OS_INPUT_BUTTON_V;
	else if (Key == "W")
		return OS_INPUT_BUTTON_W;
	else if (Key == "X")
		return OS_INPUT_BUTTON_X;
	else if (Key == "Y")
		return OS_INPUT_BUTTON_Y;
	else if (Key == "Z")
		return OS_INPUT_BUTTON_Z;

	else if (Key == "0")
		return OS_INPUT_BUTTON_ZERO;
	else if (Key == "1")
		return OS_INPUT_BUTTON_ONE;
	else if (Key == "2")
		return OS_INPUT_BUTTON_TWO;
	else if (Key == "3")
		return OS_INPUT_BUTTON_THREE;
	else if (Key == "4")
		return OS_INPUT_BUTTON_FOUR;
	else if (Key == "5")
		return OS_INPUT_BUTTON_FIVE;
	else if (Key == "6")
		return OS_INPUT_BUTTON_SIX;
	else if (Key == "7")
		return OS_INPUT_BUTTON_SEVEN;
	else if (Key == "8")
		return OS_INPUT_BUTTON_EIGHT;
	else if (Key == "9")
		return OS_INPUT_BUTTON_NINE;
	
	else if (Key == "ESC")
		return OS_INPUT_BUTTON_ESC;
	else if (Key == "ENTER")
		return OS_INPUT_BUTTON_ENTER;
	else if (Key == "ESPACE")
		return OS_INPUT_BUTTON_ESPACE;
	else 
	{ 
		assert(false);
		return OS_INPUT_BUTTON_ZERO;
	}
}

CInputContext::~CInputContext()
{
	delete m_Conversions;
}


bool CInputContext::MapButtonToAction(SOSInputButtons Button, EAction& Out) const
{
	if (Button.m_Button == OS_INPUT_BUTTON_C)	 
	{
		int x = 0;
	}
	std::map<SOSInputButtons, EAction>::const_iterator iter = m_ActionMap.find(Button);
	
	if (iter == m_ActionMap.end())
	{
		return false;
	}
	Out = iter->second;
	return true;
}

bool CInputContext::MapButtonToState(SOSInputButtons Button, EState& Out) const
{
	std::map<SOSInputButtons, EState>::const_iterator iter = m_StateMap.find(Button);
	if (iter == m_StateMap.end())
	{
		return false;
	}
	Out = iter->second;
	return true;
}


bool CInputContext::MapAxisToRange(EOSInputAxis axis, ERange& out) const
{
	std::map<EOSInputAxis, ERange>::const_iterator iter = m_RangeMap.find(axis);
	if (iter == m_RangeMap.end())
	{
		assert(false);
		return false;
	}
	out = iter->second;
	return true;
}

double CInputContext::GetSensitivity(ERange range) const
{
	std::map<ERange, double>::const_iterator iter = m_SensitivityMap.find(range);
	if (iter == m_SensitivityMap.end())
	{
		assert(false);
		return 1.0;
	}
	return iter->second;
}

