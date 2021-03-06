#ifndef _PARTICLEMANAGER_H
#define _PARTICLEMANAGER_H

#include "Utils\TemplatedMapManager.h"
#include "Particles\ParticleSystemType.h"
#include <vector>
class CRenderManager;

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

	CParticleSystemType* GetDefaultType() const { return m_DefaultType; }

	const std::vector<CParticleSystemType *> & GetLUAParticles();

};

#endif