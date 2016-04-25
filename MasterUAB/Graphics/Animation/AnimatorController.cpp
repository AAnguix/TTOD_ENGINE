#include "AnimatorController.h"
#include "Animation\AnimatorControllerTriggerParameter.h"
#include "Animation\AnimatorControllerBoolParameter.h"
#include "RenderableObjects\RenderableObject.h"

CAnimatorController::CAnimatorController(CRenderableObject* Owner)
{
	m_PreviousState = nullptr;
	m_CurrentState = nullptr;
	m_Owner = Owner;
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

CRenderableObject* CAnimatorController::GetOwner() const
{
	return m_Owner;
}

bool CAnimatorController::AddState(const std::string &Name, const std::string &Animation, const float &Speed, const std::string &OnEnter, const std::string &OnUpdate, const std::string &OnExit)
{
	std::map<const std::string, CState*>::iterator itMap;

	itMap = m_States.find(Name);

	if (itMap != m_States.end())
	{
		return false;
	}
	else
	{
		CState* l_State = new CState(this,Name, Animation, Speed, OnEnter, OnUpdate, OnExit);

		m_States.insert(std::pair<const std::string, CState*>(Name, l_State));
		
		if (m_CurrentState == nullptr)
		{
			m_CurrentState = l_State;
			m_CurrentState->OnEnter();
		}
		
		return true;
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

bool CAnimatorController::AddInteger(const std::string &Name, const int &LaunchValue, CAnimatorControllerIntegerParameter::EIntegerCondition Condition)
{
	if (!SearchParameter(Name))
	{
		CAnimatorControllerIntegerParameter* l_IntegerParameter;
		l_IntegerParameter = new CAnimatorControllerIntegerParameter(Name, LaunchValue, Condition);
		
		m_Parameters.insert(std::pair<const std::string, CAnimatorControllerParameter*>(Name, (CAnimatorControllerParameter*)l_IntegerParameter));
		return true;
	}
	return false;
}
bool CAnimatorController::AddFloat(const std::string &Name, const float &LaunchValue, CAnimatorControllerFloatParameter::EFloatCondition Condition)
{
	if (!SearchParameter(Name))
	{
		CAnimatorControllerFloatParameter* l_FloatParameter;
		l_FloatParameter = new CAnimatorControllerFloatParameter(Name, LaunchValue, Condition);

		m_Parameters.insert(std::pair<const std::string, CAnimatorControllerParameter*>(Name, (CAnimatorControllerParameter*)l_FloatParameter));
		return true;
	}
	return false;
}
bool CAnimatorController::AddBool(const std::string &Name, const bool &LaunchValue)
{
	if (!SearchParameter(Name))
	{
		CAnimatorControllerBoolParameter* l_BoolParameter;
		l_BoolParameter = new CAnimatorControllerBoolParameter(Name, LaunchValue);

		m_Parameters.insert(std::pair<const std::string, CAnimatorControllerParameter*>(Name, (CAnimatorControllerParameter*)l_BoolParameter));
		return true;
	}
	return false;
}
bool CAnimatorController::AddTrigger(const std::string &Name)
{
	if (!SearchParameter(Name))
	{
		CAnimatorControllerTriggerParameter* l_TriggerParameter;
		l_TriggerParameter = new CAnimatorControllerTriggerParameter(Name);

		m_Parameters.insert(std::pair<const std::string, CAnimatorControllerParameter*>(Name, (CAnimatorControllerParameter*)l_TriggerParameter));
		return true;
	}
	return false;
}

void CAnimatorController::SetInteger(const std::string &Name, const int &Value)
{
	std::map<const std::string, CAnimatorControllerParameter*>::iterator itMap;
	itMap = m_Parameters.find(Name);

	if (itMap != m_Parameters.end())
	{
		CAnimatorControllerIntegerParameter* l_Param = (CAnimatorControllerIntegerParameter*)(itMap->second);
		l_Param->UpdateParameter(Value);
	}
}
void CAnimatorController::SetFloat(const std::string &Name, const float &Value)
{
	std::map<const std::string, CAnimatorControllerParameter*>::iterator itMap;
	itMap = m_Parameters.find(Name);

	if (itMap != m_Parameters.end())
	{
		CAnimatorControllerFloatParameter* l_Param = (CAnimatorControllerFloatParameter*)(itMap->second);
		l_Param->UpdateParameter(Value);
	}
}
void CAnimatorController::SetBool(const std::string &Name, const bool &Value)
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

void CAnimatorController::Update(float ElapsedTime)
{
	m_CurrentState->OnUpdate(ElapsedTime);
}
void CAnimatorController::ChangeCurrentState(CState* NewState)
{
	m_PreviousState = m_CurrentState;
	m_CurrentState->OnExit();
	m_CurrentState = NewState;
	NewState->OnEnter();
}