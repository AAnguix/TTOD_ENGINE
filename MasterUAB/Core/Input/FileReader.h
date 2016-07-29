#ifndef _FILEREADER_H
#define _FILEREADER_H

#include <assert.h>
#include <istream>

template <typename T>
T ReadValue(std::wistream& stream)
{
	T l_Output;
	if (!(stream >> l_Output))
	{
		assert(false);
	}

	return l_Output;
}

//template <typename T>
//T ReadValue(std::istream& stream)
//{
//	T l_Output;
//	if (!(stream >> l_Output))
//	{
//		assert(false);
//	}
//
//	return l_Output;
//}

#endif
