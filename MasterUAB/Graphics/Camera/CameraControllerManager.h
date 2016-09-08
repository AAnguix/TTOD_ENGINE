#ifndef _CAMERACONTROLLERMANAGER_H
#define _CAMERACONTROLLERMANAGER_H

#include "Utils\TemplatedMapManager.h"
#include "Camera\CameraController.h"
#include "Camera\Camera.h"
#include <vector>

class CCameraControllerManager : public CTemplatedMapManager<CCameraController>
{

private:
	std::vector<CCameraController *> l_CameraControllerVector; /*To list CControllers in LUA*/

	std::string m_CameraControllersFilename;
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

	void NextCameraController();

	const std::vector<CCameraController *> & GetLUACameraControllers();
};

#endif