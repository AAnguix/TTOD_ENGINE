#ifndef _PROFILERRECORD_H
#define _PROFILERRECORD_H

#include "Utils\Named.h"

class CProfilerRecord : public CNamed
{
private:
	size_t m_Count;
	float m_StartTime;
	float m_AccumulatedTime;
	bool m_Started;
public:
	CProfilerRecord(const std::string &Name);
	virtual ~CProfilerRecord();
	void Start();
	void Stop();
	void Reset();
	void RaiseError();

	float Seconds() const{ return m_AccumulatedTime; };
	int Count() const { return m_Count; };
};

#endif