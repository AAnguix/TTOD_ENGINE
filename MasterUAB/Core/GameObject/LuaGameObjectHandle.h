#ifndef _LUAGAMEOBJECTHANDLE_H
#define _LUAGAMEOBJECTHANDLE_H
#include <string>
#include "Math\Vector3.h"

class CGameObject;
class CState;
class CMaterial;

/*Encapsulates components to LUA*/
class CLuaGameObjectHandle {

private:
	CGameObject* m_GameObject;
	void AnimatorError() const;
	void ColliderError() const;
	void AudioSourceError() const;
	void GameObjectError() const;

public:
	CLuaGameObjectHandle(CGameObject* GameObject);
	~CLuaGameObjectHandle();

	const std::string &GetName();
	CGameObject* GetGameObject() const;

	/*Graphics*/
	const Vect3f & GetPosition() const;
	Vect3f GetForward() const;
	Vect3f GetUp() const;
	Vect3f GetRight() const;
	float GetYaw() const;
	float GetPitch() const;
	float GetRoll() const;
	void SetYaw(float Yaw);
	void SetPitch(float Pitch);
	void SetRoll(float Roll);
	void SetYawPitchRoll(float Yaw, float Pitch, float Roll);

	/*Animations*/
	CState* AddState(const std::string &Name, const std::string &Animation, float Speed, const std::string &OnEnter, const std::string &OnUpdate, const std::string &OnExit);
	bool AddInteger(const std::string &Name, int Value);
	bool AddFloat(const std::string &Name, float Value);
	bool AddBool(const std::string &Name, bool Value);
	bool AddTrigger(const std::string &Name, bool Value);
	void SetInteger(const std::string &Name, int Value);
	void SetFloat(const std::string &Name, float Value);
	void SetBool(const std::string &Name, bool Value);
	void SetTrigger(const std::string &Name);

	/*Physics*/
	CMaterial* GetPhysxMaterial() const;

	/*Audio*/
	bool AddSound(const std::string &Key, const std::string &SoundEventName);
	void PlayEvent(const std::string &Key);
};

#endif