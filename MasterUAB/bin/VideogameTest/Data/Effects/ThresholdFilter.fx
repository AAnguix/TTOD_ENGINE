#include "globals.fxh"
#include "samplers.fxh"
#include "functions.fxh"

static float m_Threshold=m_RawDataValues[0];

struct VS_INPUT
{
	float4 Pos : POSITION;
	float4 Color : COLOR0;
	float2 UV : TEXCOORD0;
};

struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float4 Color : COLOR0;
	float2 UV : TEXCOORD0;
};

PS_INPUT VS(VS_INPUT IN)
{
	PS_INPUT l_Output = (PS_INPUT)0;
	l_Output.Pos=IN.Pos;
	l_Output.Color=IN.Color*m_BaseColor;
	l_Output.UV=IN.UV;
	return l_Output;
}

float4 PS(PS_INPUT IN) : SV_Target
{
	float4 l_Color = T0Texture.Sample(S0Sampler, IN.UV);
	return saturate((l_Color - m_Threshold) / (1 - m_Threshold));
}



