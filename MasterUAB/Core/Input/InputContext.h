#ifndef _INPUTCONTEXT_H
#define _INPUTCONTEXT_H

#include "OSInputConstants.h"
#include "InputConstants.h"
#include "Input\RangeConverter.h"
#include <map>

namespace InputMapping
{

	class CInputContext
	{

	public:
	/*	explicit CInputContext(const std::wstring& contextfilename);*/
		explicit CInputContext(const std::string &ContextFileName);
		~CInputContext();

	public:
		bool MapButtonToAction(SOSInputButtons Button, EAction& Out) const;
		bool MapButtonToState(SOSInputButtons  Button, EState& Out) const;
		bool MapAxisToRange(EOSInputAxis axis, ERange& out) const;

		double GetSensitivity(ERange range) const;
		
		const CRangeConverter& GetConversions() const { return *m_Conversions; }

	private:

		InputMapping::EOSInputButtonX StringToOSButton(const std::string& Key);

		std::map<SOSInputButtons, EAction> m_ActionMap;
		std::map<SOSInputButtons, EState> m_StateMap;
		std::map<EOSInputAxis, ERange> m_RangeMap;

		std::map<ERange, double> m_SensitivityMap;
		CRangeConverter* m_Conversions;
	};
}

#endif