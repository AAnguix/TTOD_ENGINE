#ifndef _PARTICLESYSTEMISNTANCE_H
#define _PARTICLESYSTEMISNTANCE_H

class CParticleSystemType;
class CXMLTreeNode;
class CRenderableVertexs;
class CEmptyPointerClass;
class CAnimatedInstanceModel;

#include "Render\RenderManager.h"
#include "RenderableObjects\RenderableObject.h"

#include "Vertex\VertexTypes.h"
#include <random>
#include "Math\Color.h"

#define s_MaxParticlesPerInstance 250

class CParticleSystemInstance : public CRenderableObject
{

private:
	CParticleSystemType* m_Type;
	std::random_device rnd;
	Mat44f m_WorldViewProjMatrix;

	struct ParticleData
	{
		Vect3f Position, Velocity, Acceleration;
		int CurrentFrame;
		float TimeToNextFrame;
		float LifeTime, TotalLife;
		float Angle, AngularSpeed, AngularAcceleration;

		size_t ColorControlPoint, SizeControlPoint;

		float LastColorControlTime, NextColorControlTime;
		float LastSizeControlTime, NextSizeControlTime;

		CColor LastColor, NextColor;
		float LastSize, NextSize;
	};

	int m_ActiveParticles;
	ParticleData m_ParticleData[s_MaxParticlesPerInstance];
	float m_ParticleDepth[s_MaxParticlesPerInstance];

	MV_PARTICLE_VERTEX m_ParticleRenderableData[s_MaxParticlesPerInstance];
	CRenderableVertexs* m_Vertices;

	float m_NextParticleEmission;
	bool m_Awake;
	float m_AwakeTimer;

	Vect3f m_EmissionBoxHalfSize;
	float m_EmissionVolume, m_EmissionScaler;

	std::mt19937 m_RandomEngine;
	std::uniform_real_distribution<float> m_UnitDistribution;

	float GetRandomValue(float Min, float Max);
	Vect3f GetRandomValue(Vect3f Min, Vect3f Max);
	CColor GetRandomValue(CColor Min, CColor Max);
	float GetRandomValue(Vect2f Value);

	ParticleData AddParticle();

	void RGBtoHSV(float r, float g, float b, float *h, float *s, float *v);
	void HSVtoRGB(float *r, float *g, float *b, float h, float s, float v);

	//CColor RGB2HSV(Vect4f RGB);
	//CColor RGBtoHSV(CColor RGB);
	//CColor RGBtoHCV(CColor RGB);

	//CColor HSVtoRGB(CColor HSV);
	//CColor HUEtoRGB(float H);

	float ComputeTimeToNextParticle();
	void GenerateNewParticles(float ElapsedTime);
	void UpdateParticles(float ElapsedTime);

	void InsertSort();

	float GetParticleDepth(Vect3f Position);

public:

	CParticleSystemInstance(CXMLTreeNode &TreeNode);
	CParticleSystemInstance(const std::string &Name, const std::string &TypeName, CGameObject* Owner, const Vect3f &Position, float Yaw, float Pitch, float Roll);
	virtual ~CParticleSystemInstance();

	void SetEmissionScaler(float EmissionScaler){ m_EmissionScaler = EmissionScaler; };
	float GetEmissionVolume() const{ return m_EmissionVolume; };
	
	CParticleSystemType* GetType() const{ return m_Type; };
	void SetType(CParticleSystemType* Type){ m_Type = Type; };
	TRenderableObjectType GetClassType() const{ return TRenderableObjectType::PARTICLE_EMITER; };

	void Render(CRenderManager* RenderManager);
	void Update(float ElapsedTime);

private:
	void* m_EmissionBoxHalfSizeAddress;
	void* m_YawAdress;
	void* m_PitchAdress;
	void* m_RollAdress;

public:
	virtual CEmptyPointerClass* GetEmissionBoxHalfSizeLuaAddress(int Index);
};

#endif