#include "Camera\CameraControllerManager.h"
#include "Utils\Utils.h"
#include "XML\XMLTreeNode.h"
#include "Cinematics\CameraKeyController.h"
#include "Camera\FixedCameraController.h"
#include "Camera\FPSCameraController.h"
#include "Camera\ThirdPersonCameraController.h"
#include "Engine.h"
#include "Render\RenderManager.h"

CCameraControllerManager::CCameraControllerManager()
{
	m_Filename="";
	m_CurrentCameraController = nullptr;
	m_Locked=false;
}

CCameraControllerManager::~CCameraControllerManager()
{
	
}

void CCameraControllerManager::Load(const std::string &Filename)
{
	m_Filename=Filename;

	CXMLTreeNode l_XML;

	if (l_XML.LoadFile(Filename.c_str()))
	{
		CXMLTreeNode l_Cameras = l_XML["cameras"];

		if (l_Cameras.Exists())
		{
			for (int i = 0; i < l_Cameras.GetNumChildren(); ++i)
			{
				CXMLTreeNode l_Element = l_Cameras(i);

				if (l_Element.GetName() == std::string("key_camera"))
				{
					CCameraKeyController *l_CameraKeyController=new CCameraKeyController(l_Element);

					if(!AddResource(l_Element.GetPszProperty("name",""), l_CameraKeyController))
					{
						CHECKED_DELETE(l_CameraKeyController);

					}else
					{
						if(!m_CurrentCameraController)
							m_CurrentCameraController=l_CameraKeyController;
					}
				}
				else if (l_Element.GetName() == std::string("fixed_camera"))
				{
					//CFPSCameraController *l_FPSCameraController=new CFPSCameraController(l_Element);
					CThirdPersonCameraController* l_ThirdPersonCameraController = new CThirdPersonCameraController(l_Element);
					
					if(!AddResource(l_Element.GetPszProperty("name",""), l_ThirdPersonCameraController))
					{
						CHECKED_DELETE(l_ThirdPersonCameraController);
					}
					else
					{
						if(!m_CurrentCameraController)
							m_CurrentCameraController=l_ThirdPersonCameraController;
					}
				}
			}
		}
	}
}

void CCameraControllerManager::Reload()
{
	m_CurrentCameraController = nullptr;
	Destroy();
	Load(m_Filename);
}

void CCameraControllerManager::SetCurrentCameraController(const std::string CameraControllerName)
{
	CCameraController* l_Controller = GetResource(CameraControllerName);
	if(l_Controller)
	{
		m_CurrentCameraController=l_Controller;
	}
}

CCameraController* CCameraControllerManager::GetCurrentCameraController() const
{
	return m_CurrentCameraController;
}

void CCameraControllerManager::Update(float ElapsedTime)
{
	m_CurrentCameraController->Update(ElapsedTime); //Only for CCameraKeyController
	if(!m_Locked)
	{
		m_CurrentCameraController->SetCamera(&m_CurrentCamera); //Controler modifies m_CurrentCamera
		CEngine::GetSingleton().GetRenderManager()->SetCurrentCamera(m_CurrentCamera);
	}
}
 