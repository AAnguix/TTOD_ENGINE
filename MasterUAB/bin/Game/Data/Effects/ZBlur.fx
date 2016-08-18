#include "globals.fxh"
#include "samplers.fxh"
#include "functions.fxh"

static float m_ZBlurActive=m_RawDataValues[0];
static float m_ZBlurShowDepths=m_RawDataValues[1];
static float m_ZBlurConstantBlur=m_RawDataValues[2];
static float m_ZBlurFocalStart=m_RawDataValues[3];
static float m_ZBlurFocalEnd=m_RawDataValues[4];
static float m_ZBlurEnd=m_RawDataValues[5];

// static float m_ZBlurActive=1.0;
//static float m_ZBlurShowDepths=m_RawDataValues[1];
// static float m_ZBlurConstantBlur=0.1;
// static float m_ZBlurFocalStart=3.0; //3
// static float m_ZBlurFocalEnd=8.0; //8
// static float m_ZBlurEnd=14.0; //14

//--------------------------------------------------------------------------------------
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

float4 GetZBlurColor(float Distance, float4 SourceColor, float4 BlurColor)
{
	float l_Blur=1.0;
	if(Distance<m_ZBlurFocalStart)
		l_Blur=max(Distance/m_ZBlurFocalStart, m_MouseXDisplacement); //m_ZBlurConstantBlur m_MouseXDisplacement
	else if(Distance>m_ZBlurFocalEnd)
		l_Blur=max(1.0-(Distance-m_ZBlurFocalEnd)/m_ZBlurEnd, m_MouseXDisplacement);
	
	if(l_Blur<1.0)
		return BlurColor*(1-l_Blur)+l_Blur*SourceColor;
	else
		return SourceColor;
}

PS_INPUT VS( VS_INPUT IN )
{
	PS_INPUT l_Output = (PS_INPUT)0;
	l_Output.Pos=IN.Pos;
	l_Output.Color=IN.Color;
	l_Output.UV=IN.UV;
	return l_Output;
}

float4 PS(PS_INPUT IN) : SV_Target
{
	float l_Depth=T2Texture.Sample(S2Sampler, IN.UV).r;
	float3 l_WorldPosition=GetPositionFromZDepthView(l_Depth, IN.UV, m_InverseView, m_InverseProjection);
	
	float3 l_CameraPosition=m_InverseView[3].xyz;
	float l_Distance=length(l_WorldPosition-l_CameraPosition);
	
	float4 l_SourceColor =  T0Texture.Sample(S0Sampler, IN.UV);
	if(m_ZBlurActive != 1.0f)
		return float4(l_SourceColor);
	
	float4 l_BlurColor = T1Texture.Sample(S1Sampler, IN.UV);
	return GetZBlurColor(l_Distance, l_SourceColor, l_BlurColor);
}
