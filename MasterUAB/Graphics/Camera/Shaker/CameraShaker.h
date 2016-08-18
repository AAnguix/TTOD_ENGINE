#ifndef _CAMERASHAKER_H
#define _CAMERASHAKER_H

#include "Shaker.h"
#include "Math\Vector3.h"
class CCameraController;

class CCameraShaker
{

private:
	Vect3f m_StartingCameraPosition;
	float m_StartingCameraYaw;
	float m_StartingCameraPitch;
	float m_StartingCameraRoll;

	CShaker m_YawShaker;
	CShaker m_PitchShaker;

	int m_Timer;
	int m_Duration;
	bool m_Shaking;
	float m_Intensity;

public:
	CCameraShaker();
	/*void Initialize(const std::string &CameraShakerFileName);*/

	void Start(Vect3f StartingCameraRotation, int ShakeDuration, int YawFrequency, float MinYawRandom, float MaxYawRandom, int PitchFrequency, float MinPitchRandom, float MaxPitchRandom);
	
	void Update(float ElapsedTime, CCameraController* Camera);
	void Stop(float& CameraYaw, float& CameraPitch);
	bool Finish() const { return !m_Shaking; }
	
};

#endif

