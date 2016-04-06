#include "Log.h"
#include <iostream>
#include <ctime>
#include <time.h>
#include <errno.h>
#include <string.h>
#include <locale.h>
#include <stdio.h>

CLog::CLog()
{

}

CLog::~CLog()
{
	if(m_LogFile.is_open())
	{
		m_LogFile << "--------------------------------------------------";
		m_LogFile.close();
	}
}

void CLog::Initialize(bool Clear)
{
	if(Clear)
		m_LogFile.open ("Data/Log.txt", std::ofstream::out| std::ofstream::trunc); 
	else m_LogFile.open ("Data/Log.txt", std::ios::app);
	
	m_LogFile << "\n";

	struct tm newtime;
	__time32_t aclock;

	char buffer[32];
	errno_t errNum;

	_time32( &aclock ); 
	 _localtime32_s( &newtime, &aclock ); 
	 
	errNum = asctime_s(buffer, 32, &newtime);
	if (errNum)
	{
		char buff[100];
		strerror_s(buff,100,errNum);
		Log(buff);
	}

	m_LogFile << buffer;
	m_LogFile << "--------------------------------------------------" << "\n";
	m_LogFile.flush();
}

void CLog::Log(const std::string &Text)
{
	m_LogFile << "-->" << Text;
	m_LogFile << "\n";
	m_LogFile.flush();
}

void CLog::Log(float Value)
{
	std::ostringstream ostr;
	ostr << Value;
	std::string value = ostr.str();

	m_LogFile << "-->" << value;
	m_LogFile << "\n";
	m_LogFile.flush();
}

void CLog::Log(int Value)
{
	std::ostringstream ostr;
	ostr << Value;
	std::string value = ostr.str();

	m_LogFile << "-->" << value;
	m_LogFile << "\n";
	m_LogFile.flush();
}

void CLog::Log(const Vect3f& Value)
{
	std::ostringstream ostr;
	ostr << Value.x << "," << Value.y << "," << Value.z;
	std::string value = ostr.str();

	m_LogFile << "-->" << value;
	m_LogFile << "\n";
	m_LogFile.flush();
}

void CLog::Erase()
{
	m_LogFile.clear();
}

void CLog::Reload(bool Clear)
{
	if(m_LogFile.is_open())
	{
		m_LogFile << "--------------------------------------------------";
		m_LogFile.close();

		Initialize(Clear);
	}
}