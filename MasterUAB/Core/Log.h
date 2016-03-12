#ifndef _LOG_H
#define _LOG_H
#include <string>
#include <fstream>

class CLog
{
private:
	std::ofstream m_LogFile;

public:
	CLog();
	virtual ~CLog();
	void Initialize(bool Clear);
	void Reload(bool Clear);

	void Log(int Value);
	void Log(float Value);
	void Log(const std::string &Text);
	void Erase();
};

#endif