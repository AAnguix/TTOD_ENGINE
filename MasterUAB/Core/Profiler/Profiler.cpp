#include "Profiler\Profiler.h"
#include "Engine\Engine.h"

CProfiler::CProfiler()
:m_StartTime(0.0f)
,m_NextOutputTime(5.0f)
,m_NumFrames(0)
{

}

CProfiler::~CProfiler()
{

}

void CProfiler::Initialize()
{
	m_StartTime = CEngine::GetSingleton().GetRealTimeSinceStartup();
	m_DisplayText = "\n\n Wait...";
}

void CProfiler::Begin(const std::string ID)
{
	std::map<const std::string, CProfilerRecord*>::iterator it;
	bool l_Exists = false;

	for (it = m_Resources.begin(); it != m_Resources.end(); ++it)
	{
		if (it->first == ID) { l_Exists = true; }
	}

	if (!l_Exists)
		AddResource(ID, new CProfilerRecord(ID));

	m_Resources[ID]->Start();
}
void CProfiler::End(const std::string ID)
{
	m_Resources[ID]->Stop();
}
void CProfiler::Update()
{
	m_NumFrames++;

	if (CEngine::GetSingleton().GetRealTimeSinceStartup() > m_NextOutputTime)
	{
		int colWidth = 10;

		// the overall frame time and frames per second:
		m_DisplayText = "\n\n";
		float totalMS = (CEngine::GetSingleton().GetRealTimeSinceStartup() - m_StartTime) * 1000;
		float avgMS = (totalMS / m_NumFrames);
		float fps = (1000 / (totalMS / m_NumFrames));
		/*m_DisplayText += "Avg frame time: ";
		m_DisplayText += avgMS.ToString("0.#") + "ms, ";
		m_DisplayText += fps.ToString("0.#") + " fps \n";*/

	/*	m_DisplayText += "Total".PadRight(colWidth);
		m_DisplayText += "MS/frame".PadRight(colWidth);
		m_DisplayText += "Calls/fra".PadRight(colWidth);
		m_DisplayText += "MS/call".PadRight(colWidth);
		m_DisplayText += "Label";
		m_DisplayText += "\n";*/

		std::map<const std::string, CProfilerRecord*>::iterator it;

		for (it = m_Resources.begin(); it != m_Resources.end(); ++it)
		{
			CProfilerRecord* recording = it->second;
			float recordedMS = (recording->Seconds() * 1000);
			float percent = (recordedMS * 100) / totalMS;
			float msPerFrame = recordedMS / m_NumFrames;
			float msPerCall = recordedMS / recording->Count();
			float timesPerFrame = recording->Count() / (float)m_NumFrames;

			/*m_DisplayText += (percent.ToString("0.000") + "%").PadRight(colWidth);
			m_DisplayText += (msPerFrame.ToString("0.000") + "ms").PadRight(colWidth);
			m_DisplayText += (timesPerFrame.ToString("0.000")).PadRight(colWidth);
			m_DisplayText += (msPerCall.ToString("0.0000") + "ms").PadRight(colWidth);
			m_DisplayText += (recording.id);
			m_DisplayText += "\n";*/

			recording->Reset();
		}
		
		//Debug.Log(displayText);

		m_NumFrames = 0;
		m_StartTime = CEngine::GetSingleton().GetRealTimeSinceStartup();
		m_NextOutputTime = CEngine::GetSingleton().GetRealTimeSinceStartup() + 5.0f;
	}
}