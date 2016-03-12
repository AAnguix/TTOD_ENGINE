#ifndef _APPLICATION_H
#define _APPLICATION_H

#include "Camera/SphericalCameraController.h"
#include "Camera/FPSCameraController.h"
#include "HelperTypes.h"

#include "Render\RenderManager.h"
#include "Materials\MaterialManager.h"

#include "AntTweakBar.h"
#include <string>


class CContextManager;

class CApplication
{

public:
	CApplication(CContextManager *_ContextManager);
	~CApplication();

	void SwitchCamera();

	void Update(float ElapsedTime);
	void Render();
	void Initialize();

	void LUAReload(float ElapsedTime);
	void LUAControlCharacter(float ElapsedTime);

private:

	Vect3f CalcForward(float Yaw);

	//CSphericalCameraController m_SphericalCamera;
	//CFPSCameraController m_FPSCamera;

	CCamera m_Camera;

	int m_CurrentCamera;

	CContextManager *m_ContextManager;

	CColor m_BackgroundColor;

	CRenderManager m_RenderManager;
};

#endif