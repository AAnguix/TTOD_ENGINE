#ifndef _STATE_H
#define _STATE_H

#include <string>
#include <map>
#include <vector>
#include "Animation\AnimatorControllerParameter.h"
class CAnimatorController;

class CState 
{

private:
	CAnimatorController* m_AnimatorController;

	struct STransition
	{
		CState* m_NewState;
		std::vector<CAnimatorControllerParameter*> m_Parameters;
		bool m_HasExitTime;
		float m_ExitTime;
		STransition(CState* NewState, const bool &HasExitTime, const float &ExitTime);
		~STransition();
	};

	std::string m_Name;
	float m_Speed;
	std::string m_Animation;
	std::map<const std::string, STransition*> m_Transitions;

	std::string m_OnEnter;
	std::string m_OnUpdate;
	std::string m_OnExit;

	bool TransitionMeetsConditions(CState::STransition* Transition);

public:
	CState(CAnimatorController*, const std::string &Name, const std::string &Animation, const float &Speed, const std::string &OnEnter, const std::string &OnUpdate, const std::string &OnExit);
	virtual ~CState();
	void AddTransition(const std::string &Name, CState* NewState, const bool &HasExitTime, const float &ExitTime);
	CAnimatorControllerParameter* AddTransitionParameter();
	void OnEnter();
	void OnUpdate(float ElapsedTime);
	void OnExit();
};

#endif