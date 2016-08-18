#ifndef _STATE_H
#define _STATE_H

#include <string>
#include <map>
#include <vector>
#include "Components\AnimatorController\AnimatorControllerIntegerParameter.h"
#include "Components\AnimatorController\AnimatorControllerFloatParameter.h"
#include "Components\AnimatorController\AnimatorControllerBoolParameter.h"
#include "Components\AnimatorController\AnimatorControllerTriggerParameter.h"
#include "Components\AnimatorController\Animation.h"

class CTransition;
class CAnimatorController;

class CState 
{

private:
	CAnimatorController* m_AnimatorController;

	std::string m_Name;
	float m_Speed;
	std::vector<EAnimation> m_Animations;
	unsigned int m_CurrentAnimationIndex;
	std::map<const std::string, CTransition*> m_Transitions;

	std::string m_OnEnter;
	std::string m_OnUpdate;
	std::string m_OnExit;
	bool CheckStateChange(bool HasExitTime, float Timer, CState* NewState, CTransition* Transition);

	float m_LastTimeSinceStart;
	float m_RestartAnimationsTime;

public:
	CState(CAnimatorController* AnimatorController, const std::string &Name, const EAnimation &Animation, float Speed, const std::string &OnEnter, const std::string &OnUpdate, const std::string &OnExit);
	CState(CAnimatorController* AnimatorController, const std::string &Name, const std::vector<EAnimation> &Animations, float RestartAnimationsTime, float Speed, const std::string &OnEnter, const std::string &OnUpdate, const std::string &OnExit);
	virtual ~CState();
	EAnimation GetAnimation(unsigned int Id = 0) const { return m_Animations[Id]; }
	CTransition* AddTransition(const std::string &Name, CState* NewState, bool HasExitTime, float DelayIn, float DelayOut);
	CTransition* AddTransition(const std::string &Name, CState* NewState, bool HasExitTime, float DelayIn);
	void OnEnter(CTransition* Transition);
	void OnUpdate(float ElapsedTime);
	void OnExit(CTransition* Transition);

	CAnimatorController* GetOwnAnimatorController() const { return m_AnimatorController; };
};

#endif