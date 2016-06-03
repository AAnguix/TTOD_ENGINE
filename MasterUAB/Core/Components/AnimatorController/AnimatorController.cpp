#include "AnimatorController.h"
#include "Components\AnimatorController\AnimatorControllerTriggerParameter.h"
#include "Components\AnimatorController\AnimatorControllerBoolParameter.h"
#include "Animation\AnimatedInstanceModel.h"
#include "Components\AnimatorController\Animation.h"
#include "Animation\AnimatedCoreModel.h"

CAnimatorController::CAnimatorController(const std::string &Name, CRenderableObject* Owner)
:CComponent(Name,Owner)
,m_PreviousState(nullptr)
,m_CurrentState(nullptr)
{
}

CState* CAnimatorController::GetState(const std::string &Name)
{
	std::map<const std::string, CState*>::iterator it;

	it = m_States.find(Name);
	if (it == m_States.end())
		return nullptr;
	else
		return it->second;
}

CAnimatorControllerParameter* CAnimatorController::GetParameter(const std::string &Name)
{
	std::map<const std::string, CAnimatorControllerParameter*>::iterator it;

	it = m_Parameters.find(Name);
	if (it == m_Parameters.end())
		return nullptr;
	else
		return it->second;
}

CAnimatorController::~CAnimatorController()
{
	std::map<const std::string, CState*>::iterator itMap;

	for (itMap = m_States.begin(); itMap != m_States.end(); ++itMap)
	{
		delete itMap->second;
	}

	m_States.clear();

	std::map<const std::string, CAnimatorControllerParameter*>::iterator itMapParams;

	for (itMapParams = m_Parameters.begin(); itMapParams != m_Parameters.end(); ++itMapParams)
	{
		delete itMapParams->second;
	}

	m_Parameters.clear();
}

void CAnimatorController::Update(float ElapsedTime)
{
	m_CurrentState->OnUpdate(ElapsedTime);
}

void CAnimatorController::Render(CRenderManager &RenderManager)
{

}

void CAnimatorController::RenderDebug(CRenderManager &RenderManager)
{

}


CState* CAnimatorController::AddState(const std::string &Name, const std::string &Animation, float Speed, const std::string &OnEnter, const std::string &OnUpdate, const std::string &OnExit)
{
	std::map<const std::string, CState*>::iterator itMap;

	itMap = m_States.find(Name);

	if (itMap != m_States.end())
	{
		return nullptr;
	}
	else
	{	
	 	EAnimation l_Animation = ((CAnimatedInstanceModel*)m_Owner)->m_AnimatedCoreModel->GetAnimation(Animation);

		CState* l_State = new CState(this, Name, l_Animation, Speed, OnEnter, OnUpdate, OnExit);

		m_States.insert(std::pair<const std::string, CState*>(Name, l_State));
		
		if (m_CurrentState == nullptr)
		{
			m_CurrentState = l_State;
			m_CurrentState->OnEnter(nullptr);
		}
		
		return l_State;
	}
}
bool CAnimatorController::SearchParameter(const std::string &Name)
{
	std::map<const std::string, CAnimatorControllerParameter*>::iterator itMap;

	itMap = m_Parameters.find(Name);
	
	if (itMap != m_Parameters.end())
	{
		return true;
	}
	
	return false;
}

bool CAnimatorController::AddInteger(const std::string &Name, int Value)
{
	if (!SearchParameter(Name))
	{
		CAnimatorControllerIntegerParameter* l_IntegerParameter;
		l_IntegerParameter = new CAnimatorControllerIntegerParameter(Name, Value);
		
		m_Parameters.insert(std::pair<const std::string, CAnimatorControllerParameter*>(Name, (CAnimatorControllerParameter*)l_IntegerParameter));
		return true;
	}
	return false;
}
bool CAnimatorController::AddFloat(const std::string &Name, float Value)
{
	if (!SearchParameter(Name))
	{
		CAnimatorControllerFloatParameter* l_FloatParameter;
		l_FloatParameter = new CAnimatorControllerFloatParameter(Name, Value);

		m_Parameters.insert(std::pair<const std::string, CAnimatorControllerParameter*>(Name, (CAnimatorControllerParameter*)l_FloatParameter));
		return true;
	}
	return false;
}
bool CAnimatorController::AddBool(const std::string &Name, bool Value)
{
	if (!SearchParameter(Name))
	{
		CAnimatorControllerBoolParameter* l_BoolParameter;
		l_BoolParameter = new CAnimatorControllerBoolParameter(Name, Value);

		m_Parameters.insert(std::pair<const std::string, CAnimatorControllerParameter*>(Name, (CAnimatorControllerParameter*)l_BoolParameter));
		return true;
	}
	return false;
}
bool CAnimatorController::AddTrigger(const std::string &Name, bool Value)
{
	if (!SearchParameter(Name))
	{
		CAnimatorControllerTriggerParameter* l_TriggerParameter;
		l_TriggerParameter = new CAnimatorControllerTriggerParameter(Name, Value);

		m_Parameters.insert(std::pair<const std::string, CAnimatorControllerParameter*>(Name, (CAnimatorControllerParameter*)l_TriggerParameter));
		return true;
	}
	return false;
}

void CAnimatorController::SetInteger(const std::string &Name, int Value)
{
	std::map<const std::string, CAnimatorControllerParameter*>::iterator itMap;
	itMap = m_Parameters.find(Name);

	if (itMap != m_Parameters.end())
	{
		CAnimatorControllerIntegerParameter* l_Param = (CAnimatorControllerIntegerParameter*)(itMap->second);
		l_Param->UpdateParameter(Value);
	}
}
void CAnimatorController::SetFloat(const std::string &Name, float Value)
{
	std::map<const std::string, CAnimatorControllerParameter*>::iterator itMap;
	itMap = m_Parameters.find(Name);

	if (itMap != m_Parameters.end())
	{
		CAnimatorControllerFloatParameter* l_Param = (CAnimatorControllerFloatParameter*)(itMap->second);
		l_Param->UpdateParameter(Value);
	}
}
void CAnimatorController::SetBool(const std::string &Name, bool Value)
{
	std::map<const std::string, CAnimatorControllerParameter*>::iterator itMap;
	itMap = m_Parameters.find(Name);

	if (itMap != m_Parameters.end())
	{
		CAnimatorControllerBoolParameter* l_Param = (CAnimatorControllerBoolParameter*)(itMap->second);
		l_Param->UpdateParameter(Value);
	}
}
void CAnimatorController::SetTrigger(const std::string &Name)
{
	std::map<const std::string, CAnimatorControllerParameter*>::iterator itMap;
	itMap = m_Parameters.find(Name);

	if (itMap != m_Parameters.end())
	{
		CAnimatorControllerTriggerParameter* l_Param = (CAnimatorControllerTriggerParameter*)(itMap->second);
		l_Param->TriggerEnter();
	}
}

void CAnimatorController::ChangeCurrentState(CState* NewState, CTransition* Transition)
{
	m_PreviousState = m_CurrentState;
	m_CurrentState->OnExit(Transition);
	m_CurrentState = NewState;
	NewState->OnEnter(Transition);
}