#ifndef _CAMERACONTROLLER_H
#define _CAMERACONTROLLER_H

#include "Math\Matrix44.h"
#include "Math\Vector3.h"
#include <string>

class CCamera;

class CCameraController
{
protected:
	Vect3f m_Position;

	float m_Yaw;
	float m_Pitch;

	enum EType
	{
		FIXED=0,
		KEY,
		COUNT
	};

public:
	CCameraController();
	virtual ~CCameraController();
	virtual void SetCamera(CCamera *Camera) const = 0;
	virtual void AddYaw(float Radians);
	virtual void AddPitch(float Radians);
	void SetYaw(float Yaw) {m_Yaw=Yaw;}
	float GetYaw() const {return m_Yaw;}
	void SetPitch(float Pitch) {m_Pitch=Pitch;}
	float GetPitch() const {return m_Pitch;}
	Vect3f GetRight() const;
	Vect3f GetUp() const;
	virtual Vect3f GetForward() const;
	const Vect3f & GetPosition() const { return m_Position; }
	void SetPosition(const Vect3f &Position) { m_Position = Position; }

	virtual EType GetType() const;

	virtual void Update(float ElapsedTime)
	{
	}
};

#endif
