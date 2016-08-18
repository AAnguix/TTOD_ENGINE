#ifndef _CAMERASMOOTHER_H
#define _CAMERASMOOTHER_H

#include "Math\Vector3.h"
#include "Math\Quaternion.h"

class CCameraController;

class CCameraSmoother
{
private:
	bool m_Enabled;
	float m_Velocity;
	float m_FrequencyInverse;

public:
	CCameraSmoother();
	void Start(Vect3f StartingCameraPosition, Vect3f CameraStartRotation, Vect3f Distance, Quatf Rotation, float Speed, float Dismish, int Shakes);
	void Update(float ElapsedTime, CCameraController* Camera);
	void Stop();
	
};

#endif

