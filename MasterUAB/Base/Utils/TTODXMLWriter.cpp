#include "Utils\TTODXMLWriter.h"
#include <assert.h>

bool CTTODXMLWriter::WriteTabs()
{
	assert(m_File != NULL);
	if (m_File == NULL)
		return false;
	for (int i = 0; i<m_CurrentTabs; ++i)
		fprintf(m_File,"\t");
	return true;
}

CTTODXMLWriter::CTTODXMLWriter()
:m_File(NULL)
,m_CurrentTabs(0)
,m_FirstElementWritten(false)
,m_LastWriteElementsCount(0)
,m_LastElementHasChilds(false)
,m_LastElementHasProperties(false)
{
	
}

void CTTODXMLWriter::StartFile(const std::string &Filename)
{
	errno_t l_Error;
	l_Error = fopen_s(&m_File, Filename.c_str(), "w+t");
	assert(l_Error == 0);
	fprintf(m_File, "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>");
}

bool CTTODXMLWriter::StartElement(const std::string &ElemName, bool HasProperties)
{

	assert(m_File != NULL);
	if (m_File == NULL)
		return false;

	if (!m_FirstElementWritten)
		fprintf(m_File, "%s", "\n");

	int l_ChildTop = 0;

	m_Elems.push(ElemName);
	if (!m_Childs.empty())
	{
		m_LastElementHasChilds = true;
		int l_CurrentChilds = m_Childs.top();
		m_Childs.pop();

		m_Childs.push(l_CurrentChilds + 1);
		l_ChildTop = m_Childs.top();
	}
	m_Childs.push(0);
	
	//if (m_LastWriteElementsCount == 1 && m_FirstElementWritten && m_LastElementHasChilds)
	//if (m_FirstElementWritten && m_LastElementHasChilds && m_LastElementHasProperties && m_LastWriteElementsCount == 1)
	if (m_FirstElementWritten && m_LastElementHasChilds && m_LastElementHasProperties && l_ChildTop==1)
	{
		fprintf(m_File, ">");
		fprintf(m_File, "\n");
	}
	m_LastWriteElementsCount = m_Elems.size();
	
	//if (m_Elems.size()==2 && m_Childs.size() == 2)
	//fprintf(m_File, ">");

	WriteTabs();
	if (HasProperties)
	{
		m_LastElementHasProperties = true;
		fprintf(m_File, "<%s", ElemName.c_str());
	}
	else 
	{
		m_LastElementHasProperties = false;
		fprintf(m_File, "<%s>", ElemName.c_str());
		fprintf(m_File, "\n");
	}
	++m_CurrentTabs;

	m_FirstElementWritten = true;

	return true;
}
bool CTTODXMLWriter::EndElement()
{
	assert(m_File != NULL);
	if (m_File == NULL)
		return false;

	int l_ChildsCount = m_Childs.top();
	m_Childs.pop();

	std::string l_ElementName = m_Elems.top();
	m_Elems.pop();

	if (l_ChildsCount == 0)
	{
		m_LastElementHasChilds = false;
		fprintf(m_File, "/>\n");
	}
	else
		fprintf(m_File, "</%s>\n", l_ElementName.c_str());
	--m_CurrentTabs;
	return true;
}

bool CTTODXMLWriter::WriteStringProperty(const std::string &PropertyName, const std::string &Value)
{
	assert(m_File != NULL);
	if (m_File == NULL)
		return false;
	fprintf(m_File, " %s=\"%s\"", PropertyName.c_str(), Value.c_str());
	return true;
}
bool CTTODXMLWriter::WriteBoolProperty(const std::string &PropertyName, bool Value)
{
	assert(m_File != NULL);
	if (m_File == NULL)
		return false;
	fprintf(m_File, " %s=\"%s\"", PropertyName.c_str(), Value ? "true" : "false");
	return true;
}
bool CTTODXMLWriter::WriteIntProperty(const std::string &PropertyName, int Value)
{
	assert(m_File != NULL);
	if (m_File == NULL)
		return false;
	fprintf(m_File, " %s=\"%d\"", PropertyName.c_str(), Value);
	return true;
}

bool CTTODXMLWriter::WriteFloatProperty(const std::string &PropertyName, float Value)
{
	assert(m_File != NULL);
	if (m_File == NULL)
		return false;
	fprintf(m_File, " %s=\"%f\"", PropertyName.c_str(), Value);
	return true;
}

bool CTTODXMLWriter::WriteVect2fProperty(const std::string &PropertyName, const Vect2f &Value)
{
	assert(m_File != NULL);
	if (m_File == NULL)
		return false;
	fprintf(m_File, " %s=\"%f", PropertyName.c_str(), Value.x);
	fprintf(m_File, " %f\"", Value.y);
	return true;
}

bool CTTODXMLWriter::WriteVect3fProperty(const std::string &PropertyName, const Vect3f &Value)
{
	assert(m_File != NULL);
	if (m_File == NULL)
		return false;
	fprintf(m_File, " %s=\"%f", PropertyName.c_str(), Value.x);
	fprintf(m_File, " %f", Value.y);
	fprintf(m_File, " %f\"", Value.z);
	return true;
}

bool CTTODXMLWriter::WriteVect4fProperty(const std::string &PropertyName, const Vect4f &Value)
{
	assert(m_File != NULL);
	if (m_File == NULL)
		return false;
	fprintf(m_File, " %s=\"%f", PropertyName.c_str(), Value.x);
	fprintf(m_File, " %f", Value.y);
	fprintf(m_File, " %f", Value.z);
	fprintf(m_File, " %f\"", Value.w);
	return true;
}

bool CTTODXMLWriter::EndFile()
{
	assert(m_File != NULL);
	if (m_File == NULL)
		return false;
	fclose(m_File);
	return true;
}