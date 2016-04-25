#ifndef _ANIMATORCONTROLLER_H
#define _ANIMATORCONTROLLER_H

#include "State.h"
#include "Animation\AnimatorControllerParameter.h"
#include "Animation\AnimatorControllerIntegerParameter.h"
#include "Animation\AnimatorControllerFloatParameter.h"
class CRenderableObject;

class CAnimatorController 
{

private:
	std::map<const std::string, CAnimatorControllerParameter*> m_Parameters;
	std::map<const std::string, CState*> m_States;
	CState* m_CurrentState;
	CState* m_PreviousState;
	CRenderableObject* m_Owner;

	bool SearchParameter(const std::string &Name);
	
public:
	CAnimatorController(CRenderableObject* Owner);
	CRenderableObject* GetOwner() const;
	virtual ~CAnimatorController();
	bool AddState(const std::string &Name, const std::string &Animation, const float &Speed, const std::string &OnEnter, const std::string &OnUpdate, const std::string &OnExit);

	bool AddInteger(const std::string &Name, const int &LaunchValue, CAnimatorControllerIntegerParameter::EIntegerCondition Condition);
	bool AddFloat(const std::string &Name, const float &LaunchValue, CAnimatorControllerFloatParameter::EFloatCondition Condition);
	bool AddBool(const std::string &Name, const bool &LaunchValue);
	bool AddTrigger(const std::string &Name);

	void SetInteger(const std::string &Name, const int &Value);
	void SetFloat(const std::string &Name, const float &Value);
	void SetBool(const std::string &Name, const bool &Value);
	void SetTrigger(const std::string &Name);
	
	void Update(float ElapsedTime);
	void ChangeCurrentState(CState* NewState);
};

#endif