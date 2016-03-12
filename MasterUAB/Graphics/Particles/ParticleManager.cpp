#include "Particles\ParticleManager.h"
#include "XML\XMLTreeNode.h"

CParticleManager::CParticleManager() : m_Filename("")
{
	
}

void CParticleManager::Load(const std::string &Filename)
{
	CXMLTreeNode l_File;
	m_Filename = Filename;
	if (l_File.LoadFile(m_Filename.c_str()))
	{
		CXMLTreeNode l_Meshes = l_File["particle_systems"];
		for (int i = 0; i < l_Meshes.GetNumChildren(); ++i)
		{
			CParticleSystemType *l_ParticleSystemType = new CParticleSystemType(l_Meshes(i));
			if (!AddResource(l_ParticleSystemType->GetName(), l_ParticleSystemType))
			{
				CHECKED_DELETE(l_ParticleSystemType);
			}
		}
	}
}

void CParticleManager::Reload()
{
	Destroy();
	Load(m_Filename);
}

const std::vector<CParticleSystemType *> & CParticleManager::GetLUAParticles()
{
	l_ParticlesVector.clear();
	CTemplatedMapManager<CParticleSystemType>::TMapResource &l_ParticlesMap = GetResourcesMap();
	for (CTemplatedMapManager<CParticleSystemType>::TMapResource::iterator it = l_ParticlesMap.begin(); it != l_ParticlesMap.end(); ++it)
		l_ParticlesVector.push_back(it->second);

	return l_ParticlesVector;
}