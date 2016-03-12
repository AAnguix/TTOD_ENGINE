#include "Cinematics\CinematicPlayer.h"

CCinematicPlayer::CCinematicPlayer()
{
	m_Playing=false; 
	m_CurrentTime=0.0f; 
	m_Duration=0.0f; 
	m_Cycle=false; 
}

CCinematicPlayer::~CCinematicPlayer()
{

}

void CCinematicPlayer::Init(float Duration)
{
	m_Duration=Duration;
}

void CCinematicPlayer::Update(float ElapsedTime){}
void CCinematicPlayer::Stop(){}
void CCinematicPlayer::Play(bool Cycle)
{
	m_Cycle=Cycle;
	m_Playing=true;
} 
void CCinematicPlayer::Pause(){}
void CCinematicPlayer::OnRestartCycle(){}
