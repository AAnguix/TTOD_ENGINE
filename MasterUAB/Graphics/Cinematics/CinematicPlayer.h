#ifndef _CINEMATICPLAYER_H
#define _CINEMATICPLAYER_H

class CCinematicPlayer 
{ 
protected: 
	bool m_Playing; 
	float m_CurrentTime; 
	float m_Duration; 
	bool m_Cycle; 

public: 
	CCinematicPlayer(); 
	virtual ~CCinematicPlayer(); 
	void Init(float Duration); 
	virtual void Update(float ElapsedTime); 
	virtual void Stop(); 
	virtual void Play(bool Cycle); 
	virtual void Pause(); 
	bool IsFinished() {return m_CurrentTime>=m_Duration;} 
	float GetDuration() {return m_Duration;} 
	float GetActualTime() {return m_CurrentTime;} 
	void SetActualTime(float ActualTime){ m_CurrentTime = ActualTime; };
	bool GetPlaying() const{ return m_Playing; };
	virtual void OnRestartCycle(); //Evento que se lanza cuando vuelve a empezar la cinemática
	
	
};

#endif