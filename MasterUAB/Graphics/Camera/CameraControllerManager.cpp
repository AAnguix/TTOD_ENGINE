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
		bool l_Default = false;

		if (l_Cameras.Exists())
		{
			for (int i = 0; i < l_Cameras.GetNumChildren(); ++i)
			{
				CXMLTreeNode l_Element = l_Cameras(i);

				CCameraController* l_CameraController = nullptr;
				l_Default = l_Element.GetBoolProperty("default", false);

				if (l_Element.GetName() == std::string("key_camera"))
				{
					l_CameraController = new CCameraKeyController(l_Element);	
				}
				else if (l_Element.GetName() == std::string("fps_camera"))
				{
					l_CameraController = new CFPSCameraController(l_Element);
				}
				else if (l_Element.GetName() == std::string("third_person_camera"))
				{
					l_CameraController = new CThirdPersonCameraController(l_Element);
				}
				else if (l_Element.GetName() == std::string("fixed_camera"))
				{
					l_CameraController = new CFixedCameraController(l_Element);
				}

				if (l_CameraController != nullptr)
				{
					if (!AddResource(l_Element.GetPszProperty("name", ""), l_CameraController))
					{
						CHECKED_DELETE(l_CameraController);
					}
					else
					{
						if (l_Default)
							m_CurrentCameraController = l_CameraController;
					}
				}
			}
		}
		else { assert(false); }
	}
	else { assert(false); }
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
	assert(l_Controller != nullptr);
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
 
const std::vector<CCameraController *> & CCameraControllerManager::GetLUACameraControllers()
{
	l_CameraControllerVector.clear();
	CTemplatedMapManager<CCameraController>::TMapResource &l_CControllersMap = GetResourcesMap();
	for (CTemplatedMapManager<CCameraController>::TMapResource::iterator it = l_CControllersMap.begin(); it != l_CControllersMap.end(); ++it)
		l_CameraControllerVector.push_back(it->second);

	return l_CameraControllerVector;
}