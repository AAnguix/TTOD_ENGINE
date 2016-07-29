#ifndef _INPUTRANGECONVERTER_H
#define _INPUTRANGECONVERTER_H

#include <map>
#include "Input\InputConstants.h"
class CXMLTreeNode;

namespace InputMapping
{
	class CRangeConverter
	{

	private:
		struct SConverter
		{
			double m_MinimumInput;
			double m_MaximumInput;
			double m_MinimumOutput;
			double m_MaximumOutput;

			template <typename RangeType>
			RangeType Convert(RangeType invalue) const
			{
				double v = static_cast<double>(invalue);
				if(v < m_MinimumInput)
					v = m_MinimumInput;
				else if(v > m_MaximumInput)
					v = m_MaximumInput;

				double l_Interpolationfactor = (v - m_MinimumInput) / (m_MaximumInput - m_MinimumInput);
				return static_cast<RangeType>((l_Interpolationfactor  * (m_MaximumOutput - m_MinimumOutput)) + m_MinimumOutput);
			}
		};

	private:
		typedef std::map<ERange, SConverter> ConversionMapT;

	public:
		explicit CRangeConverter(std::wifstream& infile);
		explicit CRangeConverter(CXMLTreeNode &RangeConverterNode);

	public:
		template <typename RangeType>
		RangeType Convert(ERange Rangeid, RangeType InValue) const
		{
			ConversionMapT::const_iterator iter = ConversionMap.find(Rangeid);
			if(iter == ConversionMap.end())
				return InValue;

			return iter->second.Convert<RangeType>(InValue);
		}

	private:
		ConversionMapT ConversionMap;
	};

}

#endif

