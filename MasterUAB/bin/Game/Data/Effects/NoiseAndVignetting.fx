#include "globals.fxh"
#include "functions.fxh"
#include "samplers.fxh"


static float m_NoisePct=m_RawDataValues[0];
static float m_VignettingPct=m_RawDataValues[1];
static float m_NoiseAmount=m_RawDataValues[2];
static float m_Active=m_RawDataValues[3];

struct VS_INPUT
{
	float4 Pos : POSITION;
	float4 Color : COLOR0;
	float2 UV : TEXCOORD0;
};
struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float2 UV : TEXCOORD0;
};

PS_INPUT VS( VS_INPUT IN )
{
	PS_INPUT l_Output = (PS_INPUT)0;
	l_Output.Pos=IN.Pos;
	l_Output.UV=IN.UV;
	return l_Output;
}

float4 PS( PS_INPUT IN) : SV_Target
{ 
	float l_NoiseX=0.1*sin(IN.UV.x*IN.UV.y+0.1);
	l_NoiseX=fmod(l_NoiseX, 8)*fmod(l_NoiseX, 4);
	float l_DistortX=fmod(l_NoiseX, m_NoiseAmount);
	float l_DistortY=fmod(l_NoiseX, m_NoiseAmount+0.002);
	float2 l_DistortUV=float2(l_DistortX, l_DistortY);
	float4 l_Noise=T0Texture.Sample(S0Sampler, IN.UV+l_DistortUV)*m_NoisePct;
	float4 l_Vignetting=T1Texture.Sample(S1Sampler, IN.UV)*m_VignettingPct;
	return l_Noise+l_Vignetting;
}
