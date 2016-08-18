#include "CameraSmoother.h"
#include "Camera\CameraController.h"
#include "Camera\Camera.h"
#include "Engine\Engine.h"
#include <math.h>
#include "PerlinNoise.h"
#include "Log\Log.h"

CCameraSmoother::CCameraSmoother()
:m_Velocity(0.15f) //0.05
,m_FrequencyInverse(2.0f)
,m_Enabled(false)
{

}

void CCameraSmoother::Start(Vect3f StartingCameraPosition, Vect3f StartingCameraRotation, Vect3f Distance, Quatf Rotation, float Speed, float Dismish, int Shakes)
{
	m_Enabled = true;
}

void CCameraSmoother::Update(float ElapsedTime, CCameraController* Camera)
{
	if (m_Enabled)
	{
		Vect3f l_Rotation = Vect3f(0.0, 0.0, 0.0);

		float l_V1 = CPerlinNoise::noise3_parser(Camera->GetYaw() / m_FrequencyInverse, Camera->GetPitch() / m_FrequencyInverse, 0.0f / m_FrequencyInverse)*m_Velocity;
		l_Rotation.x += l_V1;

		//float l_V2 = CPerlinNoise::noise3_parser(Camera->GetYaw() / m_FrequencyInverse, Camera->GetPitch() / m_FrequencyInverse + 1.0f, 0.0f / m_FrequencyInverse)*m_Velocity;
		//l_Rotation.y += l_V2;

		//l_Rotation.z += CPerlinNoise.noise(m_Yaw / l_Frequence, m_Pitch / l_Frequence, m_Roll / l_Frequence + 100)*l_Amplituded;

		Camera->AddYaw(l_Rotation.x);
		//Camera->AddPitch(l_Rotation.y);
		LOG(l_Rotation.x);
	}
}

void CCameraSmoother::Stop()
{
	m_Enabled = false;
}