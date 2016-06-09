#include "Particles\ParticleManager.h"
#include "XML\XMLTreeNode.h"
#include "Engine.h"
#include "RenderableObjects\LayerManager.h"
#include "RenderableObjects\RenderableObjectsManager.h"
#include "Particles\ParticleSystemInstance.h"
#include "Log\Log.h"

CParticleManager::CParticleManager() : m_Filename("")
{
	
}

CParticleManager::~CParticleManager()
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
			else
			{
				m_DefaultType = l_Meshes(i).GetBoolProperty("default_type", false) ? l_ParticleSystemType : nullptr;
			}
		}
	}
}

void CParticleManager::Reload()
{
	m_DefaultType = nullptr;
	Destroy();
	Load(m_Filename);

	CRenderableObjectsManager* l_Particles = CEngine::GetSingleton().GetLayerManager()->GetResource("particles");
	CTemplatedVectorMapManager<CRenderableObject>::TVectorResources m_ParticleSystemInstances = l_Particles->GetResourcesVector();

	for (size_t i = 0; i < m_ParticleSystemInstances.size(); ++i)
	{
		assert(m_ParticleSystemInstances[i]->GetClassType() == CRenderableObject::PARTICLE_EMITER);

		/*Reload particle system type*/

		CParticleSystemType* l_OldType = ((CParticleSystemInstance*)m_ParticleSystemInstances[i])->GetType();

		CParticleSystemType* l_NewSystemType = GetResource(l_OldType->GetName());
		((CParticleSystemInstance*)m_ParticleSystemInstances[i])->SetType(l_NewSystemType);

		((CParticleSystemInstance*)m_ParticleSystemInstances[i])->SetEmissionScaler
			(l_NewSystemType->m_EmitAbsolute ? 1 : 1.0f / ((CParticleSystemInstance*)m_ParticleSystemInstances[i])->GetEmissionVolume());
	}

	#ifdef _DEBUG
		CEngine::GetSingleton().GetLogManager()->Log("ParticleManager reloaded");
	#endif
}

const std::vector<CParticleSystemType *> & CParticleManager::GetLUAParticles()
{
	l_ParticlesVector.clear();
	CTemplatedMapManager<CParticleSystemType>::TMapResource &l_ParticlesMap = GetResourcesMap();
	for (CTemplatedMapManager<CParticleSystemType>::TMapResource::iterator it = l_ParticlesMap.begin(); it != l_ParticlesMap.end(); ++it)
		l_ParticlesVector.push_back(it->second);

	return l_ParticlesVector;
}