#ifndef _PROFILER_H
#define _PROFILER_H

#include <string>
#include "Math\Vector4.h"
#include <map>
#include "Profiler\ProfilerRecord.h"
#include "Utils\TemplatedMapManager.h"

class CProfiler : public CTemplatedMapManager<CProfilerRecord>
{

private:
	float m_StartTime;
	float m_NextOutputTime;
	int m_NumFrames;
	std::string m_DisplayText;

public:
	CProfiler();
	virtual ~CProfiler();
	void Initialize();
	void Begin(const std::string ID);
	void End(const std::string ID);
	void Update();

	
};

#endif