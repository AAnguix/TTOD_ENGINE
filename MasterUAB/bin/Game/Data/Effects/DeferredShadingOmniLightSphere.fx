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
	return float4(0.0,0.0,0.0,0.0);
}
