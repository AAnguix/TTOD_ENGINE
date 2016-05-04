#ifndef _TRANSITION_H
#define _TRANSITION_H

class CState;
#include <string>
#include <vector>

class CTransition
{

public:
	struct EFloatCondition
	{
		enum EFloatConditionType
		{
			GREATER = 0,
			LESS
		};

		EFloatConditionType m_Type;
		float m_TriggerValue;
		const std::string m_ParameterName;
		EFloatCondition(const std::string &ParameterName, const float &TriggerValue, EFloatConditionType ConditionType);
	};

	struct EIntegerCondition
	{
		enum EIntegerConditionType
		{
			GREATER = 0,
			LESS,
			EQUALS,
			NOT_EQUAL
		};

		EIntegerConditionType m_Type;
		int m_TriggerValue;
		const std::string m_ParameterName;
		EIntegerCondition(const std::string &ParameterName, const int &TriggerValue, EIntegerConditionType ConditionType);
	};

	struct EBoolCondition
	{
		bool m_TriggerValue;
		const std::string m_ParameterName;
		EBoolCondition(const std::string &ParameterName, const bool &TriggerValue);
	};

	struct ETriggerCondition
	{
		const std::string m_ParameterName;
		ETriggerCondition(const std::string &ParameterName);
	};

private:
	CState* m_NewState;
	bool m_HasExitTime;
	float m_ExitTime;
	float m_DelayIn;
	float m_DelayOut;

	std::vector<EFloatCondition*> m_FloatConditions;
	std::vector<EIntegerCondition*> m_IntegerConditions;
	std::vector<EBoolCondition*> m_BoolConditions;
	std::vector<ETriggerCondition*> m_TriggerConditions;

	bool MeetsIntegerConditions();
	bool MeetsFloatConditions();
	bool MeetsBoolConditions();
	bool MeetsTriggerConditions();

	void ClearTriggers();

public:
	CTransition(CState* NewState, const bool &HasExitTime, const float &ExitTime, const float &DelayIn, const float &DelayOut);
	~CTransition();

	void AddFloatCondition(const std::string &ParameterName, const float &TriggerValue, EFloatCondition::EFloatConditionType TriggerConditionType);
	void AddIntegerCondition(const std::string &ParameterName, const int &TriggerValue, EIntegerCondition::EIntegerConditionType TriggerConditionType);
	void AddBoolCondition(const std::string &ParameterName, const bool &TriggerValue);
	void AddTriggerCondition(const std::string &ParameterName);

	bool MeetsConditions();
	CState* GetNewState(){ return m_NewState; };
	float GetDelayIn() const{ return m_DelayIn; };
	float GetDelayOut() const{ return m_DelayOut; };
};

#endif