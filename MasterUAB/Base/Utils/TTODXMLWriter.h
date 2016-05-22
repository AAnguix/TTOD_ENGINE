#ifndef _TTODXMLWRITER_H
#define _TTODXMLWRITER_H

#include <stack>
#include <string>
#include <stdio.h>
#include "Math\Vector2.h"
#include "Math\Vector3.h"
#include "Math\Vector4.h"

class CTTODXMLWriter
{
private:
	FILE *m_File;
	std::stack<int> m_Childs;
	std::stack<std::string> m_Elems;
	int m_CurrentTabs;
	bool m_FirstElementWritten;
	int m_LastWriteElementsCount;
	bool m_LastElementHasChilds;
	
	bool WriteTabs();

public:
	CTTODXMLWriter();
	void StartFile(const std::string &Filename);
	bool StartElement(const std::string &ElemName, bool HasProperties);
	bool EndElement();

	bool WriteStringProperty(const std::string &PropertyName, const std::string &Value);
	bool WriteBoolProperty(const std::string &PropertyName, bool Value);
	bool WriteIntProperty(const std::string &PropertyName, int Value);
	bool WriteFloatProperty(const std::string &PropertyName, float Value);
	bool WriteVect2fProperty(const std::string &PropertyName, const Vect2f &Value);
	bool WriteVect3fProperty(const std::string &PropertyName, const Vect3f &Value);
	bool WriteVect4fProperty(const std::string &PropertyName, const Vect4f &Value);
	bool EndFile();
	
};

#endif