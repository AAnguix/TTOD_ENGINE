#ifndef _PARTICLESYSTEMISNTANCE_H
#define _PARTICLESYSTEMISNTANCE_H

#include "RenderableObjects\RenderableObject.h"
class CParticleSystemType;
class CXMLTreeNode;
class CRenderableVertexs;
class CColor;
class RenderManager;
#include <random>

#define s_MaxParticlesPerInstance 250

std::random_device rnd;

class CParticleSystemInstance : public CRenderableObject
{

private:
	CParticleSystemType* m_Type;

	struct ParticleData
	{
		Vect3f Position, Velocity, Acceleration;
		int CurrentFrame;
		float TimeToNextFrame;
		float LifeTime, TotalLife;
		float Angle, AngularSpeed, AngularAcceleration;
		CColor Color;
		float Size;
	};
	int m_ActiveParticles;
	ParticleData m_ParticleData[s_MaxParticlesPerInstance];
	CRenderableVertexs* m_Vertices;

	//PARTICLE_VERTEX (POS_COLOR_UV_UV2) 

	float m_NextParticleEmission;
	bool m_Awake;
	float m_AwakeTimer;

	Vect3f m_EmissionBoxHalfSize;
	float m_EmissionVolume, m_EmissionScaler;

	std::mt19937 m_RandomEngine;
	std::uniform_real_distribution<float> m_UnitDistribution;

public:

	CParticleSystemInstance(CXMLTreeNode &TreeNode);
	virtual ~CParticleSystemInstance();
	TRenderableObjectType GetClassType() const{ return TRenderableObjectType::PARTICLE_EMITER; };
	void Render(CRenderManager* RenderManager);
	void Update(float ElapsedTime);

};

#endif