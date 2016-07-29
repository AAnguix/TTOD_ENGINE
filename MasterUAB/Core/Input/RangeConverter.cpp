#include "RangeConverter.h"
#include "Input\FileReader.h"
#include <fstream>
#include "XML\XMLTreeNode.h"

using namespace InputMapping;

//
// Construct the converter and load the conversion table provided
//
CRangeConverter::CRangeConverter(std::wifstream& infile)
{
	if(!infile)
		throw std::exception("Invalid file provided to RangeConverter constructor");

	unsigned numconversions = ReadValue<unsigned>(infile);
	for(unsigned i = 0; i < numconversions; ++i)
	{
		ERange range;
		SConverter converter;

		range = static_cast<ERange>(ReadValue<unsigned>(infile));
		converter.m_MinimumInput = ReadValue<double>(infile);
		converter.m_MaximumInput = ReadValue<double>(infile);
		converter.m_MinimumOutput = ReadValue<double>(infile);
		converter.m_MaximumOutput = ReadValue<double>(infile);

		if ((converter.m_MaximumInput < converter.m_MinimumInput) || (converter.m_MaximumOutput < converter.m_MinimumOutput))
			throw std::exception("Invalid input range conversion");

		ConversionMap.insert(std::make_pair(range, converter));
	}
}

CRangeConverter::CRangeConverter(CXMLTreeNode &RangeConverterNode)
{
	for (int i = 0; i < RangeConverterNode.GetNumChildren(); ++i)
	{
		if (RangeConverterNode(i).GetName() == std::string("conversion"))
		{
			SConverter l_Converter;
			ERange l_Range = static_cast<ERange>(RangeConverterNode(i).GetIntProperty("range", 0));
			l_Converter.m_MinimumInput = RangeConverterNode(i).GetIntProperty("min_input", 0);
			l_Converter.m_MaximumInput = RangeConverterNode(i).GetIntProperty("max_input", 0);
			l_Converter.m_MinimumOutput = RangeConverterNode(i).GetIntProperty("min_output", 0);
			l_Converter.m_MaximumOutput = RangeConverterNode(i).GetIntProperty("max_output", 0);

			if ((l_Converter.m_MaximumInput < l_Converter.m_MinimumInput) || (l_Converter.m_MaximumOutput < l_Converter.m_MinimumOutput))
			{
				assert(false);
			}

			ConversionMap.insert(std::make_pair(l_Range, l_Converter));
		}
	}
}







