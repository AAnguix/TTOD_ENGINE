#include "globals.fxh"
#include "samplers.fxh"

struct VS_INPUT
{
	float4 Pos : POSITION;
	float2 UV : TEXCOORD0;
};
struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float2 UV : TEXCOORD0;
};
PS_INPUT VS(VS_INPUT IN)
{
	PS_INPUT l_Output = (PS_INPUT)0;
	l_Output.Pos=IN.Pos;
	l_Output.UV=IN.UV;
	return l_Output;
}
float4 PS(PS_INPUT IN) : SV_Target
{
	return T0Texture.Sample(S0Sampler, IN.UV);
}