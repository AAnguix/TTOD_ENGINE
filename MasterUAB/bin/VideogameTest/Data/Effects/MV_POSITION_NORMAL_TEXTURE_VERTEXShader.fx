#include "globals.fxh"
#include "LightFunctions.fxh"
#include "samplers.fxh"
//--------------------------------------------------------------------------------------
struct VS_INPUT
{
	float3 Pos : POSITION;
	float3 Normal : NORMAL;
	float2 UV : TEXCOORD0;
};
struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float2 UV : TEXCOORD0;
	float3 WorldNormal : TEXCOORD1;
	float3 WorldPos: TEXCOORD2;
};
//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT VS( VS_INPUT IN )
{
	PS_INPUT l_Output = (PS_INPUT)0;
	l_Output.Pos = mul( float4(IN.Pos.xyz,1.0), m_World ); 
	l_Output.WorldPos=l_Output.Pos.xyz;
	l_Output.Pos = mul( l_Output.Pos, m_View );
	l_Output.Pos = mul( l_Output.Pos, m_Projection );
	l_Output.UV = IN.UV;
	l_Output.WorldNormal = normalize(mul(normalize(IN.Normal).xyz, (float3x3)m_World));
	return l_Output;
}
//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS( PS_INPUT IN) : SV_Target
{
	float4 l_Texture=T0Texture.Sample(S0Sampler, IN.UV);
	float l_SpecularPower=60.0f;
	float3 l_PixelColor = GetIluminatedPixelColor(l_Texture,l_SpecularPower,IN.WorldPos,IN.WorldNormal, m_LightAmbient, MAX_LIGHTS_BY_SHADER);
	return float4(l_PixelColor,l_Texture.a);
}