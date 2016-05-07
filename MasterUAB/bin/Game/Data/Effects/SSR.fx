#include "globals.fxh"
#include "samplers.fxh"
#include "functions.fxh"

static float m_Enabled=m_RawDataValues[0];
static float m_SSRQuality=m_RawDataValues[1];
static float m_SSROpacity=m_RawDataValues[2];
static float m_SSRIncrementMultiplier=m_RawDataValues[3]; 
static float m_OffsetScreen=m_RawDataValues[4];
static float2 m_ScreenResolution=float2(m_RawDataValues[5], m_RawDataValues[6]);

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

PS_INPUT VS( VS_INPUT IN )
{
	PS_INPUT l_Output = (PS_INPUT)0;
	l_Output.Pos=IN.Pos;
	l_Output.Color=IN.Color;
	l_Output.UV=IN.UV;
	return l_Output;
}

float4 CalcSSRColor(float2 UV, float4x4 ViewProjection, float4 SourceColor, float3 WorldPosition, float3 Nn, float SsrFactor)
{
	float4 l_Color=float4(0, 0, 0, 0);
	
	float3 l_CameraToWorldPosition=WorldPosition-m_InverseView[3].xyz;
	float l_CameraToWorldDistance=length(l_CameraToWorldPosition);
	float3 l_CameraToWorldNormalized=normalize(l_CameraToWorldPosition);
	
	float3 l_ReflectedVector=normalize(reflect(l_CameraToWorldNormalized, Nn));
	if(dot(l_ReflectedVector, l_CameraToWorldNormalized)<0)
		return SourceColor;
		
	float4 l_ScreenPosition;
	float i=0;
	float3 l_RayTrace=WorldPosition;
	float l_CurrentWorldDistance, l_RayDistance;
	float incr = 1 / m_SSRQuality;
	float3 l_CameraPosition=m_InverseView[3].xyz;
	do
	{
		i+=0.05;
		l_RayTrace+=l_ReflectedVector*incr;
		incr*=m_SSRIncrementMultiplier;
		l_ScreenPosition=mul(float4(l_RayTrace, 1), ViewProjection);
		l_ScreenPosition.xyz/=l_ScreenPosition.w;
		l_ScreenPosition.x=(l_ScreenPosition.x+1)*0.5;
		l_ScreenPosition.y=(1-l_ScreenPosition.y)*0.5;
		float2 l_ScreenCoords=l_ScreenPosition.xy;
		float l_Depth=T1Texture.Sample(S1Sampler, l_ScreenCoords).x;
		float3 l_CurrentWorldPosition=GetPositionFromZDepthView(l_Depth,
		l_ScreenCoords*float2(m_ScreenResolution.x, m_ScreenResolution.y), m_InverseView,
		m_InverseProjection);
		l_CurrentWorldDistance=length(l_CurrentWorldPosition.xyz-l_CameraPosition.xyz);
		l_RayDistance=length(l_RayTrace.xyz-l_CameraPosition.xyz);
		if(l_ScreenPosition.x>1 || l_ScreenPosition.x<-1 || l_ScreenPosition.y>1 ||
		l_ScreenPosition.y<-1 || i>=0.5 || l_CameraToWorldDistance>l_CurrentWorldDistance)
		break;
	} while(l_RayDistance<l_CurrentWorldDistance);
	
	l_Color=T0Texture.Sample(S0Sampler, l_ScreenPosition.xy);
	float l_SSRContrib=0.0;
	
	if(l_ScreenPosition.x>1 || l_ScreenPosition.x<0 || l_ScreenPosition.y>1 ||	l_ScreenPosition.y<0)
		l_SSRContrib=1.0;
	else if(l_CameraToWorldDistance>l_CurrentWorldDistance)
		l_SSRContrib=1.0;
	else
	{
		float l_SSRContribX=l_ScreenPosition.x<0.5 ? (1.0-min(1.0,
		l_ScreenPosition.x/m_OffsetScreen)) : max(0.0, (l_ScreenPosition.y-(1.0f-m_OffsetScreen))/m_OffsetScreen);
		float l_SSRContribY=l_ScreenPosition.y<0.5 ? (1.0-min(1.0,
		l_ScreenPosition.y/m_OffsetScreen)) : max(0.0, (l_ScreenPosition.y-(1.0f-m_OffsetScreen))/m_OffsetScreen);
		l_SSRContrib=max(l_SSRContribX, l_SSRContrib);
		l_SSRContrib=max(l_SSRContribY, l_SSRContrib);
	}
	
	l_Color=SourceColor*l_SSRContrib+l_Color*(1-l_SSRContrib);
	//return float4(SsrFactor,0.0,0.0,1.0);
	return l_Color;
	
	//Porc reflejo * color reflejado + color real pixel
	//establecer tresold, cuando el color es negro, y no hacerlo por debajo de el.
}

float4 PS(PS_INPUT IN) : SV_Target
{
	float l_SsrFactor = T2Texture.Sample(S2Sampler, IN.UV.xy).w;
	
	float4 l_Color=T0Texture.Sample(S0Sampler, IN.UV);
	if(m_Enabled)
	{
		float3 Nn=normalize(T2Texture.Sample(S2Sampler, IN.UV.xy).xyz * 2 - 1.);
		float l_Depth=T1Texture.Sample(S1Sampler, IN.UV.xy).x;
		float3 l_WorldPosition=GetPositionFromZDepthView(l_Depth, IN.UV, m_InverseView, m_InverseProjection);
		float4x4 l_ViewProjection=mul(m_View, m_Projection);
		return float4(CalcSSRColor(IN.UV.xy, l_ViewProjection, l_Color, l_WorldPosition, Nn, l_SsrFactor).xyz, m_SSROpacity);
	}
	clip(-1);
	return l_Color;
}


