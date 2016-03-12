#ifndef _CAMERACONTROLLERMANAGER_H
#define _CAMERACONTROLLERMANAGER_H

#include "Utils\TemplatedMapManager.h"
#include "Camera\CameraController.h"
#include "Camera\Camera.h"

class CCameraControllerManager : public CTemplatedMapManager<CCameraController>
{

private:
	std::string m_Filename;
	CCamera m_CurrentCamera;
	bool m_Locked;
	CCameraController* m_CurrentCameraController;

public:
	CCameraControllerManager();
	virtual ~CCameraControllerManager();
	void Load(const std::string &Filename);
	void Reload();
	void Update(float ElapsedTime);

	void SetCurrentCameraController(const std::string CameraControllerName);
	CCameraController* GetCurrentCameraController() const;
	
	bool GetLocked() const {return m_Locked;};
	void SetLocked(bool Value){m_Locked=Value;};
	void ChangeLockState(){m_Locked=!m_Locked;};
};

#endif