#ifndef _ANIMATORCONTROLLER_H
#define _ANIMATORCONTROLLER_H

#include "State.h"
#include "Animation\AnimatorControllerParameter.h"
#include "Animation\AnimatorControllerIntegerParameter.h"
#include "Animation\AnimatorControllerFloatParameter.h"
#include "Components\Component.h"

class CRenderableObject;

class CAnimatorController : public CComponent
{

private:
	std::map<const std::string, CAnimatorControllerParameter*> m_Parameters;
	std::map<const std::string, CState*> m_States;
	CState* m_CurrentState;
	CState* m_PreviousState;

	bool SearchParameter(const std::string &Name);
	
public:
	static CAnimatorController * AddAnimatorController(const std::string &Name, CRenderableObject *Owner);

	CAnimatorController(const std::string &Name, CRenderableObject* Owner);
	virtual ~CAnimatorController();
	void Update(float ElapsedTime);
	void Render(CRenderManager &RenderManager);
	void RenderDebug(CRenderManager &RenderManager);

	CAnimatorControllerParameter* GetParameter(const std::string &Name);
	CState* GetCurrentState() const { return m_CurrentState; };
	CState* GetPreviousState() const { return m_PreviousState; };
	CState* GetState(const std::string &Name);
	CState* AddState(const std::string &Name, const std::string &Animation, const float &Speed, const std::string &OnEnter, const std::string &OnUpdate, const std::string &OnExit);
	void ChangeCurrentState(CState* NewState, CTransition* Transition);

	bool AddInteger(const std::string &Name, const int &Value);
	bool AddFloat(const std::string &Name, const float &Value);
	bool AddBool(const std::string &Name, const bool &Value);
	bool AddTrigger(const std::string &Name, const bool &Value);

	void SetInteger(const std::string &Name, const int &Value);
	void SetFloat(const std::string &Name, const float &Value);
	void SetBool(const std::string &Name, const bool &Value);
	void SetTrigger(const std::string &Name);
	
};

#endif