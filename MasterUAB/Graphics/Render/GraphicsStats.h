#ifndef _GRAPHICSSTATS_H
#define _GRAPHICSSTATS_H


class CGraphicsStats
{
private:
	float mFps;
	float mMilliSecPerFrame;

	float m_AvgMillisecondsPerFrame;
	float m_MinMillisecondsPerFrame;
	float m_MaxMillisecondsPerFrame;
	void CheckMilliSeconds(float MilliSeconds);

public:
	CGraphicsStats();
	~CGraphicsStats();
	float GetFps() { return mFps; };
	float GetMilliSecondsPerFrame() { return mMilliSecPerFrame; };

	float GetMinMillisecondsPerFrame() { return m_MinMillisecondsPerFrame; };
	float GetAvgMillisecondsPerFrame(){ return m_AvgMillisecondsPerFrame; };
	float GetMaxMillisecondsPerFrame(){ return m_MaxMillisecondsPerFrame; };

	void Update(float Dt);
	void Display();
};

#endif