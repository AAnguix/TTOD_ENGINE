/*Draws geometry edges*/

#include "globals.fxh"
#include "samplers.fxh"
#include "functions.fxh"

static float m_OutLinesActive=m_RawDataValues[0];
static float4 m_OutLinesColor=float4(m_RawDataValues[1],m_RawDataValues[2],m_RawDataValues[3],m_RawDataValues[4]);
static float m_OutlinesLineWidth=m_RawDataValues[5];

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
	float4 lum = float4(0.30, 0.59, 0.11, 1);
	float l_Alpha = T0Texture.Sample(S0Sampler, IN.UV).a;
  // TOP ROW
	float s11 = dot(T0Texture.Sample(S0Sampler, IN.UV+float2(-1.0f / m_ScreenX, -1.0f / m_ScreenY)),lum); // LEFT
	float s12 = dot(T0Texture.Sample(S0Sampler, IN.UV+float2(0, -1.0f / m_ScreenY)),lum);  // MIDDLE
	float s13 = dot(T0Texture.Sample(S0Sampler, IN.UV+float2(1.0f / m_ScreenX, -1.0f / m_ScreenY)),lum);   // RIGHT
	
	// MIDDLE ROW
	float s21 = dot(T0Texture.Sample(S0Sampler, IN.UV + float2(1.0f / m_ScreenX, 0)), lum);                // LEFT
	// Omit center
	float s23 = dot(T0Texture.Sample(S0Sampler, IN.UV  + float2(-1.0f / m_ScreenX, 0)), lum);                // RIGHT
	
	// LAST ROW
	float s31 = dot(T0Texture.Sample(S0Sampler, IN.UV  + float2(-1.0f / m_ScreenX, 1.0f / m_ScreenY)), lum);    // LEFT
	float s32 = dot(T0Texture.Sample(S0Sampler, IN.UV + float2(0, 1.0f / m_ScreenY)), lum);              // MIDDLE
	float s33 = dot(T0Texture.Sample(S0Sampler, IN.UV + float2(1.0f / m_ScreenX, 1.0f / m_ScreenY)), lum); // RIGHT
	
	float t1 = s13 + s33 + (2 * s23) - s11 - (2 * s21) - s31;
	float t2 = s31 + (2 * s32) + s33 - s11 - (2 * s12) - s13;
	
	float4 col;
	
	if (((t1 * t1) + (t2 * t2)) > m_OutlinesLineWidth) 
	{
		col = m_OutLinesColor;
		//col = float4(0,0,0,1);
		
	} else 
	{
		clip(-1);
	}
 
	return col;
}



