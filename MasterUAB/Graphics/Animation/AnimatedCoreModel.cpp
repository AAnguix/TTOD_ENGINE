#include "AnimatedCoreModel.h"
#include "Utils\Utils.h"
#include "cal3d\error.h"
#include "Engine.h"
#include "Materials\MaterialManager.h"
#include "Materials\Material.h"
#include <fstream>
#include <iostream>
#include "cal3d\coretrack.h"

bool CAnimatedCoreModel::LoadMesh(const std::string &Filename)
{
	//int r = m_CalCoreModel->loadCoreMesh(Filename);
	//return r;
	if(m_CalCoreModel->loadCoreMesh(Filename) != -1)
		return true;

	return false;
}

bool CAnimatedCoreModel::LoadSkeleton(const std::string &Filename)
{
	//int r=m_CalCoreModel->loadCoreSkeleton(Filename);
	//return r;
	return m_CalCoreModel->loadCoreSkeleton(Filename);
}

bool CAnimatedCoreModel::LoadAnimation(const std::string &Name, const std::string &Filename)
{
	//int r=m_CalCoreModel->loadCoreAnimation(Filename);
	//return r;
	if(m_CalCoreModel->loadCoreAnimation(Filename) != -1)
		return true;

	return false;
}

CAnimatedCoreModel::CAnimatedCoreModel() : 
m_CalCoreModel(NULL),
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
		CHECKED_DELETE(m_CalCoreModel)
    }

	m_Materials.clear();
	//m_AnimationsIds.clear();
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

			for (int i = 0; i < l_Actor.GetNumChildren(); ++i)
			{
				CXMLTreeNode l_Element = l_Actor(i);

				if (l_Element.GetName() == std::string("skeleton"))
				{
					std::string s = Path + l_Element.GetPszProperty("filename");
					if(!LoadSkeleton(Path+l_Element.GetPszProperty("filename")))
						return;
				}
				else if (l_Element.GetName() == std::string("mesh"))
				{
					if(!LoadMesh(Path+l_Element.GetPszProperty("filename")))
						return;
				}
				else if (l_Element.GetName() == std::string("material"))
				{	
					CMaterial *l_Material=new CMaterial(l_Element);

					if(!CEngine::GetSingleton().GetMaterialManager()->AddResource(l_Material->GetName(), l_Material))
					{
						CHECKED_DELETE(l_Material);
					}
					else
					{
						m_Materials.push_back(l_Material);
					}

				}
				else if (l_Element.GetName() == std::string("animation"))
				{
					LoadAnimation(Path+l_Element.GetPszProperty("name"),Path+l_Element.GetPszProperty("filename"));
				}
			}
		}

		
	}
}