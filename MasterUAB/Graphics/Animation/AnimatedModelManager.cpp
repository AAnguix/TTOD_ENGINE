#include "Animation\AnimatedModelManager.h"
#include "XML\XMLTreeNode.h"
#include "Utils\Utils.h"
#include "cal3d\loader.h"
#include "Log\Log.h"
#include "Engine\Engine.h"

CAnimatedModelManager::CAnimatedModelManager()
{
	CalLoader::setLoadingMode(LOADER_ROTATE_X_AXIS); /*Swaps Y/Z coordinates for all new loaded models*/
}  

CAnimatedModelManager::~CAnimatedModelManager(){}  

bool CAnimatedModelManager::Load(const std::string &Filename)
{
	m_Filename=Filename;
	
	CXMLTreeNode l_XML;

	if (l_XML.LoadFile(Filename.c_str()))
	{
		CXMLTreeNode l_AnimatedModels = l_XML["animated_models"];

		if (l_AnimatedModels.Exists())
		{
			for (int i = 0; i < l_AnimatedModels.GetNumChildren(); ++i)
			{
				CXMLTreeNode l_Element = l_AnimatedModels(i);

				if (l_Element.GetName() == std::string("animated_model"))
				{
					CAnimatedCoreModel *l_AnimatedCoreModel=new CAnimatedCoreModel();
					l_AnimatedCoreModel->SetName(l_Element.GetPszProperty("name"));

					if(!AddResource(l_AnimatedCoreModel->GetName(), l_AnimatedCoreModel))
					{
						CHECKED_DELETE(l_AnimatedCoreModel);
					}
					else
					{
						l_AnimatedCoreModel->Load(l_Element.GetPszProperty("path"));
					}
				}
			}
			return true;
		}
	}
	return false;
}

bool CAnimatedModelManager::Reload()
{
	Destroy();
	bool l_Result = Load(m_Filename);

	#ifdef _DEBUG
		CEngine::GetSingleton().GetLogManager()->Log("AnimatedModelManager reloaded");
	#endif

	return l_Result;
}

const std::vector<CAnimatedCoreModel *> & CAnimatedModelManager::GetLUAAnimatedModels()
{
	l_AnimatedCoreModelVector.clear();
	CTemplatedMapManager<CAnimatedCoreModel>::TMapResource &l_EffectsMap = GetResourcesMap();
	for (CTemplatedMapManager<CAnimatedCoreModel>::TMapResource::iterator it = l_EffectsMap.begin(); it != l_EffectsMap.end(); ++it)
		l_AnimatedCoreModelVector.push_back(it->second);

	return l_AnimatedCoreModelVector;
}