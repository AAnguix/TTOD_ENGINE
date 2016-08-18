#include "CameraShaker.h"
#include "Camera\CameraController.h"
#include "Camera\Camera.h"
#include "Engine\Engine.h"
#include <math.h>

#include <cstdint>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <random>

#include "Camera\ThirdPersonCameraController.h"
#include <fstream>
#include <assert.h>
#include <sstream>

/*
Class used to manage camera shakes.
*/
CCameraShaker::CCameraShaker()
:m_YawShaker(CShaker())
,m_PitchShaker(CShaker())
,m_Shaking(false)
,m_Timer(0)
,m_Duration(0)
,m_Intensity(10.0f)
{
	
}

//void CCameraShaker::Initialize(const std::string &CameraShakerFileName)
//{
//	std::ifstream l_CameraShakerFile;
//	l_CameraShakerFile.open(CameraShakerFileName, std::ios_base::in);
//	if (!l_CameraShakerFile)
//		assert(false);
//	else
//	{
//		unsigned int i = 0;
//		for (std::string l_Line; std::getline(l_CameraShakerFile, l_Line);)
//		{
//			std::istringstream l_StringStream(l_Line);
//			std::string l_RandomValuesFile;
//			l_StringStream >> l_RandomValuesFile;
//
//			if (i % 2 == 0)
//				m_YawShaker.AddRandoms(l_RandomValuesFile);
//			else m_PitchShaker.AddRandoms(l_RandomValuesFile);
//			++i;
//		}
//	}
//}

/* StartingCameraRotation[in]: Camera rotation when the shake starts.
   ShakeDuration[in]: The time that the camera is going to be shaking.
   ShakeFrequency[in]: Herz.
*/
void CCameraShaker::Start(Vect3f StartingCameraRotation, int ShakeDuration, int YawFrequency, float MinYawRandom, float MaxYawRandom, int PitchFrequency, float MinPitchRandom, float MaxPitchRandom)
{
	m_Duration = ShakeDuration;

	unsigned int l_File = std::rand()%4;
	m_YawShaker.Start("./Data/Cameras/ShakerRandoms/yaw_randoms_" + std::to_string(l_File) + ".txt");
	m_PitchShaker.Start("./Data/Cameras/ShakerRandoms/pitch_randoms_" + std::to_string(l_File) + ".txt");

	m_StartingCameraYaw = StartingCameraRotation.x;
	m_StartingCameraPitch = StartingCameraRotation.y;
	m_StartingCameraRoll = StartingCameraRotation.z;

	m_Shaking = true;

	//m_YawShaker.Start(ShakeDuration, YawFrequency, MinYawRandom, MaxYawRandom, "./Data/YawRandoms.txt");
	//m_PitchShaker.Start(ShakeDuration, PitchFrequency, MinPitchRandom, MaxPitchRandom, "./Data/PitchRandoms.txt");
}

void CCameraShaker::Update(float ElapsedTime, CCameraController* Camera)
{
	m_Timer += (unsigned int)(ElapsedTime * 1000);

	if (m_Timer <= m_Duration)
	{
		m_YawShaker.Update(ElapsedTime);
		float l_Yaw = m_YawShaker.GetAmplitude(0);
		((CThirdPersonCameraController*)Camera)->AddYaw((l_Yaw*m_Intensity)*ElapsedTime);

		m_PitchShaker.Update(ElapsedTime);
		float l_Pitch = m_PitchShaker.GetAmplitude(0);
		((CThirdPersonCameraController*)Camera)->AddLookAtPitch((l_Pitch*m_Intensity)*ElapsedTime);
	}
	else
	{
		m_YawShaker.FinishShaking();
		m_PitchShaker.FinishShaking();
		m_Shaking = false; 
	}
}

//Resets camera default values
void CCameraShaker::Stop(float& CameraYaw, float& CameraPitch)
{
	m_Timer = 0;
	
	//CameraYaw = m_StartingCameraYaw;
	//CameraPitch = m_StartingCameraPitch;
	//CameraRoll = m_StartingCameraRoll;
}