#include "globals.fxh"
#include "samplers.fxh"
#include "functions.fxh"

static float m_BloomActive=m_RawDataValues[0];
static float m_BloomIntensity=m_RawDataValues[1];
static float m_OriginalIntensity=m_RawDataValues[2];
static float m_BloomSaturation=m_RawDataValues[3];
static float m_OriginalSaturation=m_RawDataValues[4];

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

float4 AdjustSaturation(float4 Color, float Saturation)
{
	// We define gray as the same color we used in the grayscale shader
	float l_Grey = dot(Color.xyz, float3(0.3, 0.59, 0.11));
	return lerp(l_Grey, Color, Saturation);
}

float4 PS(PS_INPUT IN) : SV_Target
{
	if(m_BloomActive == 0.0) 
	{
		return T0Texture.Sample(S0Sampler, IN.UV);
		
	} else 
	{
		float4 l_BloomColor = T1Texture.Sample(S1Sampler, IN.UV);

		// Get our original pixel from ColorMap
		float4 l_OriginalColor = T0Texture.Sample(S0Sampler, IN.UV);

		// Adjust color saturation and intensity based on the input variables to the shader
		l_BloomColor = AdjustSaturation(l_BloomColor, m_BloomSaturation) * m_BloomIntensity;
		l_OriginalColor = AdjustSaturation(l_OriginalColor, m_OriginalSaturation) * m_OriginalIntensity;
		
		// make the originalColor darker in very bright areas, avoiding these areas look burned-out
		l_OriginalColor *= (1 - saturate(l_BloomColor));
		
		// Combine the two images.
		return l_OriginalColor + l_BloomColor;
	}
}