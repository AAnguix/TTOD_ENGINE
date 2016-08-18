#include "AnimatedCoreModel.h"
#include "Log\Log.h"
#include "Engine\Engine.h"
#include "Materials\MaterialManager.h"
#include "XML\XMLTreeNode.h"
#include "cal3d\coremodel.h"
#include "cal3d\coretrack.h"

bool CAnimatedCoreModel::LoadMesh(const std::string &Filename)
{
	if(m_CalCoreModel->loadCoreMesh(Filename) != -1)
		return true;

	return false;
}

bool CAnimatedCoreModel::LoadSkeleton(const std::string &Filename)
{
	return m_CalCoreModel->loadCoreSkeleton(Filename);
}

bool CAnimatedCoreModel::LoadAnimation(const std::string &Name, const std::string &Filename, bool Loop, float Weight)
{
	int l_AnimationID = m_CalCoreModel->loadCoreAnimation(Filename);
	if (l_AnimationID != -1)
	{
		float l_Duration = m_CalCoreModel->getCoreAnimation(l_AnimationID)->getDuration();
		m_Animations.insert(m_Animations.begin() + l_AnimationID, EAnimation(Name, Loop, l_AnimationID, Weight, l_Duration));
		return true;
	}
	return false;
}

EAnimation CAnimatedCoreModel::GetAnimation(const std::string &Name)
{
	for (unsigned int i = 0; i < m_Animations.size(); ++i)
	{
		if (m_Animations[i].m_Name == Name)
			return m_Animations[i];
	}
	CEngine::GetSingleton().GetLogManager()->Log("Can't find animation " + Name + " in core model "+m_Name);
	assert(false);
	return m_Animations[0];
}

CAnimatedCoreModel::CAnimatedCoreModel() 
:m_CalCoreModel(NULL),
m_Materials(NULL), 
m_BSPosition(v3fZERO),
m_BSRadius(v3fZERO), CNamed("")
{

}

CAnimatedCoreModel::~CAnimatedCoreModel()
{
	if ( m_CalCoreModel )
    {
        // TODO: report CoreTrack memory leak problem to cal3d maintainers
        for ( int i = 0; i < m_CalCoreModel->getCoreAnimationCount(); i++ )
        {
            CalCoreAnimation* a = m_CalCoreModel->getCoreAnimation( i );
            std::list<CalCoreTrack *>& ct = a->getListCoreTrack();
			for ( std::list<CalCoreTrack *>::iterator t = ct.begin(), tEnd = ct.end(); t != tEnd; ++t )
            {
                (*t)->destroy();
                delete (*t);
            }
            ct.clear();
        }

        //Cleanup of non-auto released resources
		if (m_CalCoreModel != NULL) 
			delete(m_CalCoreModel);
		m_CalCoreModel = NULL;
    }

	m_Materials.clear();
	//m_Animations.clear();
}

CalCoreModel* CAnimatedCoreModel::GetCoreModel()
{
	return m_CalCoreModel;
}

void CAnimatedCoreModel::Load(const std::string &Path)
{
	m_Path=Path;

	CXMLTreeNode l_XML;
	std::string l_S=m_Path+"actor.xml";

	if (l_XML.LoadFile(l_S.c_str()))
	{
		CXMLTreeNode l_Actor = l_XML["actor"];

		if (l_Actor.Exists())
		{
			m_CalCoreModel = new CalCoreModel(GetName());
			CEngine::GetSingleton().GetMaterialManager()->AddMaterialsFileName(l_S);

			for (int i = 0; i < l_Actor.GetNumChildren(); ++i)
			{
				CXMLTreeNode l_Element = l_Actor(i);

				if (l_Element.GetName() == std::string("skeleton"))
				{
					std::string s = Path + l_Element.GetPszProperty("filename");
					if (!LoadSkeleton(Path + l_Element.GetPszProperty("filename")))
					{
						#ifdef _DEBUG
							LOG("Unable to load skeleton " + std::string(l_Element.GetPszProperty("filename")));
						#endif
						return;
					}
				}
				else if (l_Element.GetName() == std::string("mesh"))
				{
					if (!LoadMesh(Path + l_Element.GetPszProperty("filename")))
					{
						#ifdef _DEBUG
							LOG("Unable to load mesh " + std::string(l_Element.GetPszProperty("filename")));
						#endif
						return;
					}
				}
				else if (l_Element.GetName() == std::string("material"))
				{	
					std::string l_MaterialName = l_Element.GetPszProperty("name", "");
					CMaterial *l_Material=new CMaterial(l_Element);

					if(!CEngine::GetSingleton().GetMaterialManager()->AddResource(l_Material->GetName(), l_Material))
					{
						if (l_Material != NULL)
							delete(l_Material);
						l_Material = NULL;
						#ifdef _DEBUG
							LOG("Unable to load material " + std::string(l_MaterialName) + " on model " + m_Name + ". It's already created.");
						#endif
					}
					else
					{
						CEngine::GetSingleton().GetMaterialManager()->InsertMaterialIntoMaterialsFileName(l_MaterialName, l_S);
						m_Materials.push_back(l_Material);
					}

				}
				else if (l_Element.GetName() == std::string("animation"))
				{
					LoadAnimation(l_Element.GetPszProperty("name"),Path+l_Element.GetPszProperty("filename"),l_Element.GetBoolProperty("loop",false), l_Element.GetFloatProperty("weight",1.0f));
				}
			}
		}
	}
}

int CAnimatedCoreModel::GetBoneId(const std::string &BoneName)
{
	return m_CalCoreModel->getBoneId(BoneName); 
}