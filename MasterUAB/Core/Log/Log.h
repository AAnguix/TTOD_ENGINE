#ifndef _LOG_H
#define _LOG_H

#include <string>
#include <fstream>
#include "Math\Vector3.h"
#include "Math\Matrix44.h"

class CLog
{
private:
	std::ofstream m_LogFile;
	unsigned int m_TimeDecimalDigits;
	void AppendTimeSinceStart(std::ofstream &Stream);

public:
	CLog();
	virtual ~CLog();
	void Initialize(bool Clear);
	void Reload(bool Clear);

	void Log(int Value);
	void Log(float Value);
	void Log(const std::string& Value);
	void Log(const Vect3f& Value);
	void Log(const Mat44f& Value);

	void Erase();
};

#endif