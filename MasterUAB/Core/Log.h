#ifndef _LOG_H
#define _LOG_H
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "Math\Vector3.h"

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
	void Log(const std::string& Value);
	void Log(const Vect3f& Value);

	void Erase();
};

#endif