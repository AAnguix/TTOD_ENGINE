#include "Render\GraphicsStats.h"
#include <cstdio>

CGraphicsStats::CGraphicsStats()
:mFps(0.0f)
,mMilliSecPerFrame(0.0f)
,m_AvgMillisecondsPerFrame(0.0f)
,m_MinMillisecondsPerFrame(1000.0f)
,m_MaxMillisecondsPerFrame(0.0f)
{
}
CGraphicsStats::~CGraphicsStats()
{
}

void CGraphicsStats::CheckMilliSeconds(float MilliSeconds)
{
	if ((MilliSeconds > 0.0f) && (MilliSeconds < m_MinMillisecondsPerFrame))
	{
		m_MinMillisecondsPerFrame = MilliSeconds;
	}
	if (MilliSeconds > m_MaxMillisecondsPerFrame)
	{
		m_MaxMillisecondsPerFrame = MilliSeconds;
	}
}

//void CGraphicsStats::Update(float Dt)
//{
//	static float numFrames = 0.0f;
//	static float timeElapsed = 0.0f;
//
//	numFrames += 1.0f;
//	timeElapsed += Dt;
//
//	if (timeElapsed >= 1.0f)
//	{
//		mFps = numFrames;
//		mMilliSecPerFrame = 1000.0F / mFps;
//		CheckMilliSeconds(mMilliSecPerFrame);
//		timeElapsed = 0.0F;
//		numFrames = 0.0f;
//	}
//}

void CGraphicsStats::Update(float Dt)
{
	static float numFrames = 0.0f;
	static float timeElapsed = 0.0f;
	static float totalMilliseconds = 0.0f;
	
	numFrames += 1.0f;
	timeElapsed += Dt;

	mMilliSecPerFrame = (timeElapsed / numFrames)*1000;
	CheckMilliSeconds(mMilliSecPerFrame);
	totalMilliseconds += mMilliSecPerFrame;

	if (timeElapsed >= 1.0f)
	{
		mFps = numFrames;
		m_AvgMillisecondsPerFrame = totalMilliseconds / numFrames;
		timeElapsed = 0.0F;
		numFrames = 0.0f;
		totalMilliseconds = 0.0f;
	}
}

void CGraphicsStats::Display()
{
	static char l_Buffer[256];
	sprintf_s(l_Buffer, "FPS = %2.f\n""MPF = %.4f\n",mFps,mMilliSecPerFrame);
}