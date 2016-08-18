#ifndef _LUACOMPONENT_H
#define _LUACOMPONENT_H

#include "Components\Component.h"
class CEmptyPointerClass;

class CLUAComponent
{
private:
	std::string m_Name;
	//std::string m_Type;
	float m_Timer;
	bool m_Enabled;
public:
	virtual ~CLUAComponent() {}
	CLUAComponent(const std::string Name);
	virtual const std::string &GetName();
	//virtual std::string GetType(){ return m_Type; };

	virtual void AddTime(float Time){ m_Timer += Time; };
	virtual float GetTimer() { return m_Timer; };
	virtual void ResetTimer() { m_Timer = 0.0f; };
	
	virtual void Update(float ElapsedTime){};
	virtual void Disable(){ m_Enabled = false; };
	virtual void Enable(){ m_Enabled = true; };
	virtual bool IsEnabled(){ return m_Enabled; };

	virtual void OnTriggerEnter(const std::string& Actor){};
	virtual void OnTriggerExit(const std::string& Actor){};
	virtual void OnShapeHit(const std::string& Actor){};

	CEmptyPointerClass* GetThisLuaAddress() const;
	CEmptyPointerClass* GetEnabledLuaAddress() const;
};

#endif