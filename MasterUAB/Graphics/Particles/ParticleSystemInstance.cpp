#include "Particles\ParticleSystemInstance.h"
#include "Particles\ParticleSystemType.h"
#include "XML\XMLTreeNode.h"
#include <algorithm>
#include "Vertex\RenderableVertexs.h"
#include "RenderableObjects\RenderableObjectTechnique.h"
#include "Engine\Engine.h"
#include "ParticleManager.h"
#include "Camera\CameraControllerManager.h"
#include "Utils\EmptyPointerClass.h"
#include "Materials\Material.h"

#define s_epsilon std::pow(1.0f,-10)

CParticleSystemInstance::CParticleSystemInstance(CXMLTreeNode &TreeNode) 
:CRenderableObject(TreeNode)
,m_ActiveParticles(0), m_RandomEngine(rnd()), m_UnitDistribution(0.0f,1.0f)
,m_NextParticleEmission(0.0f), m_Awake(false), m_AwakeTimer(0.0f)
,m_Type(CEngine::GetSingleton().GetParticleSystemManager()->GetResource(TreeNode.GetPszProperty("type")))
,m_EmissionBoxHalfSize(TreeNode.GetVect3fProperty("emission_box_size", Vect3f(1, 1, 1))*0.5f)
{
	assert(m_Type != nullptr);
	m_EmissionVolume = m_EmissionBoxHalfSize.x * m_EmissionBoxHalfSize.y * m_EmissionBoxHalfSize.z * 8;
	m_EmissionScaler = m_Type->m_EmitAbsolute ? 1 : 1.0f / m_EmissionVolume;

	CEngine::GetSingleton().GetRenderManager()->GetContextManager()->SetWorldMatrix(GetTransform());

	m_WorldViewProjMatrix = CEffectManager::m_SceneEffectParameters.m_World * CEffectManager::m_SceneEffectParameters.m_View * CEffectManager::m_SceneEffectParameters.m_Projection;

	m_Vertices = new CPointListRenderableVertexs<MV_PARTICLE_VERTEX>(m_ParticleRenderableData, s_MaxParticlesPerInstance, s_MaxParticlesPerInstance, true);

		//void *Vtxs, unsigned int VtxsCount, D3D11_PRIMITIVE_TOPOLOGY PrimitiveTopology, unsigned int PrimitiveCount, bool Dynamic
		//l_RV = new CTriangleListRenderableIndexed32Vertexs<MV_POSITION_NORMAL_TEXTURE_VERTEX>(l_VtxsData, l_NumVertexs, l_IdxData, m_NumIndexs, false);
}

CParticleSystemInstance::CParticleSystemInstance(const std::string &Name, const std::string &TypeName, CGameObject* Owner, const Vect3f &Position, float Yaw, float Pitch, float Roll)
:CRenderableObject(Owner, Name, Position, Yaw, Pitch, Roll)
,m_ActiveParticles(0), m_RandomEngine(rnd()), m_UnitDistribution(0.0f, 1.0f)
,m_NextParticleEmission(0.0f), m_Awake(false), m_AwakeTimer(0.0f)
,m_Type(CEngine::GetSingleton().GetParticleSystemManager()->GetResource(TypeName))
,m_EmissionBoxHalfSize(Vect3f(1, 1, 1))
{
	assert(m_Type != nullptr);
}

CParticleSystemInstance::~CParticleSystemInstance()
{
	CHECKED_DELETE(m_Vertices);
}

void CParticleSystemInstance::Render(CRenderManager *RenderManager)
{
	for (int i = 0; i < m_ActiveParticles; ++i)
	{
		ParticleData *l_Particle = &m_ParticleData[i];

		float l_SizeControlAlpha = (l_Particle->LifeTime < l_Particle->NextSizeControlTime) ? (l_Particle->LifeTime - l_Particle->LastSizeControlTime) / (l_Particle->NextSizeControlTime - l_Particle->LastSizeControlTime) : 1.0f;
		float l_ColorControlAlpha = (l_Particle->LifeTime < l_Particle->NextColorControlTime) ? (l_Particle->LifeTime - l_Particle->LastColorControlTime) / (l_Particle->NextColorControlTime - l_Particle->LastColorControlTime) : 1.0f;


		m_ParticleRenderableData[i].Position = l_Particle->Position;
		m_ParticleRenderableData[i].Color = l_Particle->LastColor.Lerp(l_Particle->NextColor, l_ColorControlAlpha);
		m_ParticleRenderableData[i].UV.x = mathUtils::Lerp<float>(l_Particle->LastSize, l_Particle->NextSize, l_SizeControlAlpha);
		m_ParticleRenderableData[i].UV.y = l_Particle->Angle; //falta porcentaje
		m_ParticleRenderableData[i].UV2.x = (float)l_Particle->CurrentFrame;
		m_ParticleRenderableData[i].UV2.y = 0;
	}

	if (m_ActiveParticles > 0 && m_Enabled)
	{
		CMaterial*  l_Material = m_Type->GetMaterial();
		l_Material->Apply();
		RenderManager->GetContextManager()->SetWorldMatrix(GetTransform());
		m_Vertices->UpdateVertexs(m_ParticleRenderableData, s_MaxParticlesPerInstance);
		m_Vertices->Render(RenderManager, l_Material->GetRenderableObjectTechnique()->GetEffectTechnique(), (void*)&CEffectManager::m_SceneEffectParameters, m_ActiveParticles);
	}
}

void CParticleSystemInstance::Update(float ElapsedTime)
{
	//Intermitent emiter
	m_AwakeTimer -= ElapsedTime;
	while (m_AwakeTimer < 0)
	{
		m_Awake = !m_Awake;
		m_AwakeTimer += GetRandomValue(m_Awake ? m_Type->m_AwakeTime : m_Type->m_SleepTime);
	}

	GenerateNewParticles(ElapsedTime);
	
	UpdateParticles(ElapsedTime);
	
	InsertSort();

	//traducir info a formato que entienda el shader
}

float CParticleSystemInstance::ComputeTimeToNextParticle()
{
	float l_ParticlesPerSecPerM3 = GetRandomValue(m_Type->m_EmitRate);
	return m_EmissionScaler / l_ParticlesPerSecPerM3;
}

CParticleSystemInstance::ParticleData CParticleSystemInstance::AddParticle()
{
	ParticleData l_Particle;

	l_Particle.Position = GetRandomValue(-m_EmissionBoxHalfSize, m_EmissionBoxHalfSize);
	l_Particle.Velocity = GetRandomValue(m_Type->m_StartingSpeed1, m_Type->m_StartingSpeed2);
	l_Particle.Acceleration = GetRandomValue(m_Type->m_StartingAcceleration1, m_Type->m_StartingAcceleration2);
	l_Particle.Angle = GetRandomValue(m_Type->m_StartingAngle);
	l_Particle.AngularSpeed = GetRandomValue(m_Type->m_StartingAngularSpeed);
	l_Particle.AngularAcceleration = GetRandomValue(m_Type->m_AngularAcceleration);

	l_Particle.CurrentFrame = 0;
	l_Particle.TimeToNextFrame = m_Type->m_TimerPerFrame;

	l_Particle.LifeTime = 0;
	l_Particle.TotalLife = GetRandomValue(m_Type->m_Life);

	l_Particle.SizeControlPoint = 0;
	l_Particle.LastSizeControlTime = 0;
	l_Particle.LastSize = GetRandomValue(m_Type->m_ControlPointSizes[0].m_Size);
	l_Particle.NextSizeControlTime = m_Type->m_ControlPointSizes.size() < 2 ? l_Particle.TotalLife : GetRandomValue(m_Type->m_ControlPointSizes[1].m_Time);
	l_Particle.NextSize = m_Type->m_ControlPointSizes.size() < 2 ? l_Particle.LastSize : GetRandomValue(m_Type->m_ControlPointSizes[1].m_Size);

	l_Particle.ColorControlPoint = 0;
	l_Particle.LastColorControlTime = 0;
	l_Particle.LastColor = GetRandomValue(m_Type->m_ControlPointColor[0].m_Color1, m_Type->m_ControlPointColor[0].m_Color2);
	l_Particle.NextColorControlTime = m_Type->m_ControlPointColor.size() < 2 ? l_Particle.TotalLife : GetRandomValue(m_Type->m_ControlPointColor[1].m_Time);
	l_Particle.NextColor = m_Type->m_ControlPointColor.size() < 2 ? l_Particle.LastColor : GetRandomValue(m_Type->m_ControlPointColor[1].m_Color1, m_Type->m_ControlPointColor[1].m_Color2);

	return l_Particle;
}

void CParticleSystemInstance::GenerateNewParticles(float ElapsedTime)
{
	//GenerateParticles
	if (m_Awake)
	{
		m_NextParticleEmission -= ElapsedTime;
		while (m_NextParticleEmission < 0)
		{
			if (m_ActiveParticles < s_MaxParticlesPerInstance)
			{
				ParticleData l_Particle = AddParticle();
				m_ParticleData[m_ActiveParticles] = l_Particle;

				m_ParticleDepth[m_ActiveParticles] = GetParticleDepth(l_Particle.Position);

				++m_ActiveParticles;
			}

			m_NextParticleEmission += ComputeTimeToNextParticle();
		}
	}
}

float CParticleSystemInstance::GetParticleDepth(Vect3f Position)
{
	/*Vect4f l_HPos = Vect4f(Position, 1.0);
	l_HPos = l_HPos * m_WorldViewProjMatrix;
	return l_HPos.z / l_HPos.w;*/

	CCameraController* l_CController = CEngine::GetSingleton().GetCameraControllerManager()->GetCurrentCameraController();
	Vect3f l_CameraPosition = CEngine::GetSingleton().GetRenderManager()->GetCurrentCamera().GetPosition();
	Vect3f l_Diff = (Position) - l_CameraPosition;
	
	Vect3f l_Forward = l_CController->GetForward();
	float l_Depth = l_Diff * l_Forward;

	return l_Depth;
}

void CParticleSystemInstance::UpdateParticles(float ElapsedTime)
{
	for (int i = 0; i < m_ActiveParticles; ++i)
	{
		ParticleData *l_Particle = &m_ParticleData[i];

		l_Particle->Position += (l_Particle->Velocity*ElapsedTime) + (0.5f*ElapsedTime*ElapsedTime*l_Particle->Acceleration);
		l_Particle->Velocity += l_Particle->Acceleration*ElapsedTime;
		
		l_Particle->Angle += (l_Particle->AngularSpeed*ElapsedTime) + (0.5f*ElapsedTime*ElapsedTime*l_Particle->AngularAcceleration);
		l_Particle->AngularSpeed = l_Particle->AngularAcceleration*ElapsedTime;
	
		m_ParticleDepth[i] = GetParticleDepth(l_Particle->Position);

		/*Control POINTS*/
		while (l_Particle->LifeTime > l_Particle->NextSizeControlTime && l_Particle->LifeTime < l_Particle->TotalLife)
		{
			++l_Particle->SizeControlPoint;
			
			l_Particle->LastSize = l_Particle->NextSize;
			l_Particle->LastSizeControlTime = l_Particle->NextSizeControlTime;

			if (l_Particle->SizeControlPoint + 1 < m_Type->m_ControlPointSizes.size())
			{
				l_Particle->NextSize = GetRandomValue(m_Type->m_ControlPointSizes[l_Particle->SizeControlPoint + 1].m_Size);
				l_Particle->NextSizeControlTime = GetRandomValue(m_Type->m_ControlPointSizes[l_Particle->SizeControlPoint+1].m_Time);
			}
			else
			{
				l_Particle->NextSizeControlTime = l_Particle->TotalLife;
			}
		}

		/*Control Points Color*/
		while (l_Particle->LifeTime > l_Particle->NextColorControlTime && l_Particle->LifeTime < l_Particle->TotalLife)
		{
			++l_Particle->ColorControlPoint;

			l_Particle->LastColor = l_Particle->NextColor;
			l_Particle->LastColorControlTime = l_Particle->NextColorControlTime;

			if (l_Particle->ColorControlPoint + 1 < m_Type->m_ControlPointColor.size())
			{
				/*Color interpolation*/
				l_Particle->NextColor = GetRandomValue(m_Type->m_ControlPointColor[l_Particle->ColorControlPoint + 1].m_Color1, m_Type->m_ControlPointColor[l_Particle->ColorControlPoint + 1].m_Color2);
				l_Particle->NextColorControlTime = GetRandomValue(m_Type->m_ControlPointColor[l_Particle->ColorControlPoint + 1].m_Time);
			}
			else
			{
				l_Particle->NextColorControlTime = l_Particle->TotalLife;
			}
		}
		/*END*/


		l_Particle->TimeToNextFrame -= ElapsedTime;
		l_Particle->LifeTime += ElapsedTime;

		while (l_Particle->TimeToNextFrame < 0 && (m_Type->m_LoopFrames || l_Particle->CurrentFrame < m_Type->m_NumFrames - 1))
		{
			l_Particle->CurrentFrame = (l_Particle->CurrentFrame + 1) % m_Type->m_NumFrames;
			l_Particle->TimeToNextFrame += m_Type->m_TimerPerFrame;
		}

		if (m_ParticleData[i].LifeTime > m_ParticleData[i].TotalLife)
		{
			--m_ActiveParticles;
			m_ParticleData[i] = m_ParticleData[m_ActiveParticles];
			--i;
		}
	}
}


float CParticleSystemInstance::GetRandomValue(float Min, float Max)
{
	float l_a = m_UnitDistribution(m_RandomEngine);
	float l_Value = mathUtils::Lerp(Min, Max, l_a);
	return l_Value;
}

Vect3f CParticleSystemInstance::GetRandomValue(Vect3f Min, Vect3f Max)
{
	float l_a1 = m_UnitDistribution(m_RandomEngine);
	float l_a2 = m_UnitDistribution(m_RandomEngine);
	float l_a3 = m_UnitDistribution(m_RandomEngine);
	Vect3f l_Value;
	l_Value.x = mathUtils::Lerp(Min.x, Max.x, l_a1);
	l_Value.y = mathUtils::Lerp(Min.y, Max.y, l_a2);
	l_Value.z = mathUtils::Lerp(Min.z, Max.z, l_a3);
	return l_Value;
}

CColor CParticleSystemInstance::GetRandomValue(CColor Min, CColor Max)
{
	float l_a1 = m_UnitDistribution(m_RandomEngine);
	float l_a2 = m_UnitDistribution(m_RandomEngine);
	float l_a3 = m_UnitDistribution(m_RandomEngine);
	float l_a4 = m_UnitDistribution(m_RandomEngine);

	Vect3f l_MinHSV = v3fZERO;
	Vect3f l_MaxHSV = v3fZERO;
	RGBtoHSV(Min.x, Min.y, Min.z, &l_MinHSV.x, &l_MinHSV.y, &l_MinHSV.z);
	RGBtoHSV(Max.x, Max.y, Max.z, &l_MaxHSV.x, &l_MaxHSV.y, &l_MaxHSV.z);

	CColor l_HSVLerp;

	l_HSVLerp.x = mathUtils::Lerp(l_MinHSV.x, l_MaxHSV.x, l_a1);
	l_HSVLerp.y = mathUtils::Lerp(l_MinHSV.y, l_MaxHSV.y, l_a2);
	l_HSVLerp.z = mathUtils::Lerp(l_MinHSV.z, l_MaxHSV.z, l_a3);
	l_HSVLerp.w = mathUtils::Lerp(Min.w, Max.w, l_a4);

	CColor l_RGBInterpolated = CColor(v4fZERO);
	l_RGBInterpolated.w = l_HSVLerp.w;

	HSVtoRGB(&l_RGBInterpolated.x, &l_RGBInterpolated.y, &l_RGBInterpolated.z, l_HSVLerp.x, l_HSVLerp.y, l_HSVLerp.z);
	return l_RGBInterpolated;
}

float CParticleSystemInstance::GetRandomValue(Vect2f Value)
{
	return GetRandomValue(Value.x, Value.y);
}

//CColor CParticleSystemInstance::RGBtoHSV(CColor RGB)
//{
//	CColor HCV = RGBtoHCV(RGB);
//	float l_S = HCV.y / (HCV.z + s_epsilon);
//	return CColor(HCV.x, l_S, HCV.z, HCV.w);
//	return RGB;
//}

//CColor CParticleSystemInstance::RGB2HSV(Vect4f RGB) {
//	
//	
//	float l_Hue = 0.0f;
//	float l_Saturation = 0.0f;
//	float l_Value = 0.0f;
//
//	float r = RGB.x;
//	float g = RGB.y;
//	float b = RGB.z;
//
//	if (r<0 || g<0 || b<0 || r>255 || g>255 || b>255) 
//	{
//		assert(false);
//		return CColor(0.0,0.0,0.0,0.0);
//	}
//	
//	float minRGB = min(r, min(g, b));
//	float maxRGB = max(r, max(g, b));
//
//	if (minRGB == maxRGB) 
//	{
//		l_Value = minRGB;
//		return CColor(0, 0, l_Value);
//	}
//
//	float d = (r == minRGB) ? g - b : ((b == minRGB) ? r - g : b - r);
//	float h = (r == minRGB) ? 3 : ((b == minRGB) ? 1 : 5);
//	l_Hue = 60 * (h - d / (maxRGB - minRGB));
//	l_Saturation = (maxRGB - minRGB) / maxRGB;
//	l_Value = maxRGB;
//
//	return CColor(l_Hue, l_Saturation, l_Value);
//}

void CParticleSystemInstance::RGBtoHSV(float r, float g, float b, float *h, float *s, float *v)
{
	float l_Min, l_Max, delta;
	l_Min = min(r, g);
	l_Min = min(l_Min, b);

	l_Max = max(r, g);
	l_Max = max(l_Max, b);
	*v = l_Max;				// v
	delta = l_Max - l_Min;
	if (l_Max != 0)
		*s = delta / l_Max;		// s
	else {
		// r = g = b = 0		// s = 0, v is undefined
		*s = 0;
		*h = -1;
		return;
	}
	if (r == l_Max)
		*h = (g - b) / delta;		// between yellow & magenta
	else if (g == l_Max)
		*h = 2 + (b - r) / delta;	// between cyan & yellow
	else
		*h = 4 + (r - g) / delta;	// between magenta & cyan
	*h *= 60;				// degrees
	if (*h < 0)
		*h += 360;
}

void CParticleSystemInstance::HSVtoRGB(float *r, float *g, float *b, float h, float s, float v)
{
	int i;
	float f, p, q, t;
	if (s == 0) {
		// achromatic (grey)
		*r = *g = *b = v;
		return;
	}
	h /= 60;			// sector 0 to 5
	i = (int)(floor(h));
	f = h - i;			// factorial part of h
	p = v * (1 - s);
	q = v * (1 - s * f);
	t = v * (1 - s * (1 - f));
	switch (i) {
	case 0:
		*r = v;
		*g = t;
		*b = p;
		break;
	case 1:
		*r = q;
		*g = v;
		*b = p;
		break;
	case 2:
		*r = p;
		*g = v;
		*b = t;
		break;
	case 3:
		*r = p;
		*g = q;
		*b = v;
		break;
	case 4:
		*r = t;
		*g = p;
		*b = v;
		break;
	default:		// case 5:
		*r = v;
		*g = p;
		*b = q;
		break;
	}
}

//CColor CParticleSystemInstance::RGBtoHCV(CColor RGB)
//{
//	CColor P = (RGB.y < RGB.z) ? CColor(RGB.z, RGB.y, -1.0f, 2.0f / 3.0f) : CColor(RGB.y, RGB.z, 0.0, -1.0f / 3.0f);
//	CColor Q = (RGB.x < P.x) ? CColor(P.x,P.y,P.w,RGB.x) : CColor(RGB.x, P.y,P.z,P.x);
//	float C = Q.x - fmin(Q.w, Q.y);
//	float H = abs((Q.w - Q.y) / (6 * C + s_epsilon) + Q.z);
//	return CColor(H, C, Q.x, RGB.w);
//	return RGB;
//}

//CColor CParticleSystemInstance::HSVtoRGB(CColor HSV)
//{
//	CColor RGB = HUEtoRGB(HSV.x);
//
//	CColor l_v = CColor(RGB.x - 1, RGB.y - 1, RGB.z - 1, RGB.w);
//	l_v*=HSV.y;
//	l_v = CColor(l_v.x + 1, l_v.y + 1, l_v.z + 1, l_v.w);
//	return l_v*HSV.z;
//}


//CColor CParticleSystemInstance::HUEtoRGB(float H)
//{
//	float R = abs(H * 6 - 3) - 1;
//	float G = 2 - abs(H * 6 - 2);
//	float B = 2 - abs(H * 6 - 4);
//	return CColor(R, G, B, 1.0).Clamp();
//}

void CParticleSystemInstance::InsertSort()
{
	for (int i = 0; i < m_ActiveParticles; ++i)
	{
		int j = i;

		while (j > 0 && (m_ParticleDepth[j-1] < m_ParticleDepth[j]))
		{
			float l_Depth = m_ParticleDepth[j-1];
			m_ParticleDepth[j - 1] = m_ParticleDepth[j];
			m_ParticleDepth[j] = l_Depth;

			ParticleData l_Val = m_ParticleData[j - 1];
			m_ParticleData[j - 1] = m_ParticleData[j];
			m_ParticleData[j] = l_Val;

			--j;
		}
	}
}

CEmptyPointerClass* CParticleSystemInstance::GetEmissionBoxHalfSizeLuaAddress(int Index) { return (CEmptyPointerClass *)&m_EmissionBoxHalfSize[Index]; }