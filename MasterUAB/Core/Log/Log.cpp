#include "Log\Log.h"
#include <ctime>
#include <sstream>
#include "Engine\Engine.h"

CLog::CLog()
:m_TimeDecimalDigits(4)
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

void CLog::AppendTimeSinceStart(std::ofstream &Stream)
{
	std::stringstream ss;
	//ss.precision(m_TimeDecimalDigits);
	ss << "[" << CEngine::GetSingleton().GetRealTimeSinceStartup() << "] ";
	m_LogFile << ss.str();
}

void CLog::Log(const std::string &Text)
{
	AppendTimeSinceStart(m_LogFile);
	m_LogFile << Text;
	m_LogFile << "\n";
	m_LogFile.flush();
}

void CLog::Log(float Value)
{
	std::ostringstream ostr;
	ostr << Value;
	std::string value = ostr.str();

	AppendTimeSinceStart(m_LogFile);
	m_LogFile << value;
	m_LogFile << "\n";
	m_LogFile.flush();
}

void CLog::Log(int Value)
{
	std::ostringstream ostr;
	ostr << Value;
	std::string value = ostr.str();

	AppendTimeSinceStart(m_LogFile);
	m_LogFile << value;
	m_LogFile << "\n";
	m_LogFile.flush();
}

void CLog::Log(const Vect3f& Value)
{
	std::ostringstream ostr;
	ostr << Value.x << "," << Value.y << "," << Value.z;
	std::string value = ostr.str();

	AppendTimeSinceStart(m_LogFile);
	m_LogFile << value;
	m_LogFile << "\n";
	m_LogFile.flush();
}

void CLog::Log(const Mat44f& Value)
{
	std::ostringstream ostr;
	ostr << "[" << Value.m00 << "," << Value.m01 << "," << Value.m02 << "," << Value.m03
		<< "/n" << Value.m10 << "," << Value.m11 << "," << Value.m12 << "," << Value.m13
		<< "/n" << Value.m20 << "," << Value.m21 << "," << Value.m22 << "," << Value.m23
		<< "/n" << Value.m30 << "," << Value.m31 << "," << Value.m32 << "," << Value.m33 << "]";
	std::string value = ostr.str();

	AppendTimeSinceStart(m_LogFile);
	m_LogFile << value;
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