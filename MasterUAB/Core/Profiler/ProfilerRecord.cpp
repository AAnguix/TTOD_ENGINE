#include "Profiler\ProfilerRecord.h"
#include "Engine\Engine.h"
#include "Log\Log.h"

CProfilerRecord::CProfilerRecord(const std::string &Name)
:CNamed(Name)
,m_Count(0)
,m_StartTime(0.0f)
,m_AccumulatedTime(0.0f)
,m_Started(false)
{
}
CProfilerRecord::~CProfilerRecord()
{

}
void CProfilerRecord::Start()
{
	if (m_Started){	RaiseError(); }
	m_Count++;
	m_Started = true;
	m_StartTime = CEngine::GetSingleton().GetRealTimeSinceStartup();
}
void CProfilerRecord::Stop()
{
	if (!m_Started) { RaiseError(); }
	m_Started = false;
	float l_ElapsedTime = (CEngine::GetSingleton().GetRealTimeSinceStartup() - m_StartTime);
	m_AccumulatedTime += l_ElapsedTime;
	CEngine::GetSingleton().GetLogManager()->Log(m_Name + ". Time: " + std::to_string(m_AccumulatedTime));
}
void CProfilerRecord::Reset()
{
	m_AccumulatedTime = 0.0f;
	m_Count = 0;
	m_Started = false;
}
void CProfilerRecord::RaiseError()
{
	CEngine::GetSingleton().GetLogManager()->Log("Record "+m_Name+" start/stops not balanced");
}

