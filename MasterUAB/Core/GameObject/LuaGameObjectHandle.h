#ifndef _LUAGAMEOBJECTHANDLE_H
#define _LUAGAMEOBJECTHANDLE_H
#include <string>
#include "Math\Vector3.h"
#include <vector>

class CGameObject;
class CState;
class CMaterial;
class CAnimatedCoreModel;

class CRenderableObjectTechnique;
class CTransition;

class C3DElement;

/*Encapsulates components to LUA*/
class CLuaGameObjectHandle {

private:
	CGameObject* m_GameObject;
	void AnimatorError() const;
	void ColliderError() const;
	void AudioSourceError() const;
	void RenderableObjectError() const;
	void GameObjectError() const;

public:
	CLuaGameObjectHandle(CGameObject* GameObject);
	~CLuaGameObjectHandle();

	const std::string GetName();
	CGameObject* GetGameObject() const;

	/*Graphics*/
	void EnableRenderableObject(bool Value);
	const Vect3f & GetPosition() const;
	void SetPosition(const Vect3f& Position);
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
	CAnimatedCoreModel* GetAnimatedCoreModel();
	void SetParent(CLuaGameObjectHandle* Parent, const std::string &BoneName);
	Mat44f GetBoneTransformationMatrix(const int BoneID) const;
	const Mat44f & GetTransform();
	std::vector<CMaterial*> CreateCopyMaterialsFromCore();
	void SetTemporalRenderableObjectTechnique(CRenderableObjectTechnique* RenderableObjectTechnique);

	/*Animations*/
	void EnableAnimatorController(bool Value);
	CState* AddState(const std::string &Name, const std::string &Animation, float Speed, const std::string &OnEnter, const std::string &OnUpdate, const std::string &OnExit);
	CState* AddState(const std::string &Name, std::vector<const std::string> Animations, float RestartAnimationsTime, float Speed, const std::string &OnEnter, const std::string &OnUpdate, const std::string &OnExit);
	CState* GetState(const std::string &Name);
	CTransition* AddAnyStateTransition(const std::string &Name, CState* NewState, bool HasExitTime, float DelayIn, float DelayOut);
	CTransition* AddAnyStateTransition(const std::string &Name, CState* NewState, bool HasExitTime, float DelayIn);

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
	void EnableAudioSource(bool Value);
	bool AddSound(const std::string &Key, const std::string &SoundEventName);
	void PlayEvent(const std::string &Key);
	void PlayEvent(const std::string &Key, const std::string& Speaker);
	void PlayEvent(const std::string &Key, const C3DElement* Speaker);
	C3DElement* Get3DElement() const;
};

#endif