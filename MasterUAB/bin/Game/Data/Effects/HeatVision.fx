/*Used to simulate dragon vision*/

#include "globals.fxh"
#include "samplers.fxh"
#include "functions.fxh"

static float m_HeatVisionActive=m_RawDataValues[0];
static float m_LuminanceType=m_RawDataValues[1];

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

float4 PS(PS_INPUT IN) : SV_Target
{
  float3 l_Texture = T0Texture.Sample(S0Sampler, IN.UV).rgb;
  if (m_HeatVisionActive==1.0)
  {
		float3 l_Colors[3];
		l_Colors[0] = float3(0.0,0.0,1.0);
		l_Colors[1] = float3(1.0,1.0,0.0);
		l_Colors[2] = float3(1.0,0.0,0.0);
		float lum = 0.0;
		if (m_LuminanceType==0.0)
		{
			float3 l_Correction = float3(0.30, 0.59, 0.11);
			//float3 l_Correction = float3(0.222, 0.707, 0.071);
			lum = dot(l_Correction, l_Texture.rgb);
		}
		else
		{		
			lum = (l_Texture.r+l_Texture.g+l_Texture.b)/3.0;
		}		
		int l_X = (lum < 0.5)? 0:1;
		l_Texture = lerp(l_Colors[l_X],l_Colors[l_X+1],(lum-float(l_X)*0.5)/0.5);
  }
  return float4(l_Texture, 1.0);
}


