#include "globals.fxh"
#include "functions.fxh"
#include "LightFunctions.fxh"
#include "samplers.fxh"

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
	//float2 UV : TEXCOORD0;
	float4 ScreenPos : TEXCOORD0;
};
PS_INPUT VS(VS_INPUT IN)
{
	PS_INPUT l_Output = (PS_INPUT)0;
	
	l_Output.Pos = mul( float4(IN.Pos.xyz,1.0), m_World ); 
	//l_Output.WorldPos=l_Output.Pos.xyz;
	l_Output.Pos = mul( l_Output.Pos, m_View );
	l_Output.Pos = mul( l_Output.Pos, m_Projection );
	l_Output.ScreenPos = l_Output.Pos;
	//l_Output.Normal = normalize(mul(normalize(IN.Normal).xyz, (float3x3)m_World));

	l_Output.Color=IN.Color;
	//l_Output.UV=IN.UV;
	
	return l_Output;
}

float4 PS(PS_INPUT IN) : SV_Target
{
	float2 l_ScreenPos = IN.ScreenPos.xy/IN.ScreenPos.w;
	//x=-1.left, x=1.right, y=1.top, y=-1.bottom
	float l_Width = m_Times.z;
	float l_Height = m_Times.w;
	
	float2 l_Value = (l_ScreenPos.x/l_Width,l_ScreenPos.y/l_Height);

	float4 l_DiffuseMap = T0Texture.Sample(S0Sampler, l_Value);
	float4 l_NormalMap = T1Texture.Sample(S1Sampler, l_Value);
	float l_Depth = T2Texture.Sample(S2Sampler, l_Value).r;

	float l_SpecularFactor = l_DiffuseMap.a;
	float l_Gloss = 1/(T3Texture.Sample(S3Sampler, l_Value).a);
	
	float3 l_WorldPos=GetPositionFromZDepthView(l_Depth, l_Value,m_InverseView, m_InverseProjection);
	float3 l_WorldNormal = normalize(Texture2Normal(l_NormalMap.xyz));
	float l_ShadowMapContrib = GetShadowMapContrib(0,l_WorldPos,T6Texture,S6Sampler);
	
	float3 l_DiffuseLight;
	float3 l_SpecularLight;

	GetSingleIluminatedPixelColor(0, l_DiffuseMap, l_SpecularFactor, l_Gloss, l_WorldPos, l_WorldNormal, l_DiffuseLight, l_SpecularLight, l_ShadowMapContrib);
	return float4(l_DiffuseLight+l_SpecularLight, 0.0);
}
