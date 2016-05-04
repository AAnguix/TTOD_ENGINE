#ifndef _STATE_H
#define _STATE_H

#include <string>
#include <map>
#include <vector>
#include "Animation\AnimatorControllerIntegerParameter.h"
#include "Animation\AnimatorControllerFloatParameter.h"
#include "Animation\AnimatorControllerBoolParameter.h"
#include "Animation\AnimatorControllerTriggerParameter.h"
#include "Animation\Animation.h"

class CTransition;
class CAnimatorController;

class CState 
{

private:
	CAnimatorController* m_AnimatorController;

	std::string m_Name;
	float m_Speed;
	EAnimation m_Animation;
	std::map<const std::string, CTransition*> m_Transitions;

	std::string m_OnEnter;
	std::string m_OnUpdate;
	std::string m_OnExit;

public:
	CState(CAnimatorController*, const std::string &Name, const EAnimation &Animation, const float &Speed, const std::string &OnEnter, const std::string &OnUpdate, const std::string &OnExit);
	virtual ~CState();
	CTransition* AddTransition(const std::string &Name, CState* NewState, const bool &HasExitTime, const float &ExitTime, const float &DelayIn, const float &DelayOut);
	void OnEnter(CTransition* Transition);
	void OnUpdate(float ElapsedTime);
	void OnExit(CTransition* Transition);

	CAnimatorController* GetOwnAnimatorController() const { return m_AnimatorController; };
};

#endif