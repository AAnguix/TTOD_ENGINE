#ifndef _PARTICLEMANAGER_H
#define _PARTICLEMANAGER_H

#include "Utils\TemplatedMapManager.h"
#include "Particles\ParticleSystemType.h"
class CRenderManager;
#include <vector>

class CParticleManager : public CTemplatedMapManager<CParticleSystemType>
{

private:
	std::string m_Filename;
	CParticleSystemType* m_DefaultType;
	std::vector<CParticleSystemType*> l_ParticlesVector;

public:
	CParticleManager();
	virtual ~CParticleManager();
	void Reload();
	void Update(float ElapsedTime);
	void Render(CRenderManager *RM);
	void Load(const std::string &FileName);

	UAB_GET_PROPERTY_POINTER(CParticleSystemType, DefaultType);

	const std::vector<CParticleSystemType *> & GetLUAParticles();

};

#endif