#include "globals.fxh"
#include "samplers.fxh"

static bool m_Active=m_RawDataValues[0]==1.0;
static float m_Exposure=m_RawDataValues[1];
static float m_Decay=m_RawDataValues[2];
static float m_Density=m_RawDataValues[3];
static float m_Weight=m_RawDataValues[4];
static int m_NumSamples=(int)m_RawDataValues[5];
static float2 m_LightScatteringPosition=float2(m_RawDataValues[6], m_RawDataValues[7]);


// static bool m_Active=1.0;
// static float m_Exposure=0.082;
// static float m_Decay=0.945;
// static float m_Density=0.89;
// static float m_Weight=1.0;
// static int m_NumSamples=20;
// static float2 m_LightScatteringPosition=float2(0.5,0.5);

struct VS_INPUT
{
	float4 Pos : POSITION;
	float4 Color : COLOR0;
	float2 UV : TEXCOORD0;
};
struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float2 UV : TEXCOORD0;
};

PS_INPUT VS( VS_INPUT IN )
{
	PS_INPUT l_Output = (PS_INPUT)0;
	l_Output.Pos=IN.Pos;
	l_Output.UV=IN.UV;
	return l_Output;
}

void Calc2DLinearFunction(float2 PosA, float2 PosB, out float m, out float b)
{
	float Ax=PosB.x-PosA.x;
	float Ay=PosB.y-PosA.y;
	m=Ay/Ax;
	b=PosA.y-m*PosA.x;
}
float CalcPosY2DLinearFunction(float x, float m, float b)
{
	return m*x+b;
}
float CalcPosX2DLinearFunction(float y, float m, float b)
{
	return (y-b)/m;
}
float2 ClampLightScatteringPosition(float2 UV, float2 LightScatteringPosition)
{
	float2 l_LightScatteringPosition=LightScatteringPosition;
	float l_mLinearFunction;
	float l_bLinearFunction;
	Calc2DLinearFunction(UV, LightScatteringPosition, l_mLinearFunction,l_bLinearFunction);
	
	float l_LightScatteringPositionRight=CalcPosY2DLinearFunction(1.0, l_mLinearFunction,l_bLinearFunction);
	float l_LightScatteringPositionLeft=CalcPosY2DLinearFunction(0.0, l_mLinearFunction,l_bLinearFunction);
	float l_LightScatteringPositionTop=CalcPosX2DLinearFunction(0.0, l_mLinearFunction,l_bLinearFunction);
	float l_LightScatteringPositionBottom=CalcPosX2DLinearFunction(1.0, l_mLinearFunction,l_bLinearFunction);
	float4 l_Color=float4(1,1,1,1);
	
	if(l_LightScatteringPosition.x<0.0 || l_LightScatteringPosition.y<0.0 ||
	l_LightScatteringPosition.x>1.0 || l_LightScatteringPosition.y>1.0)
	{
		if(l_LightScatteringPositionRight>=0 && l_LightScatteringPositionRight<=1.0 && LightScatteringPosition.x>=UV.x)
			l_LightScatteringPosition=float2(1.0, l_LightScatteringPositionRight);
		else if(l_LightScatteringPositionLeft>=0 && l_LightScatteringPositionLeft<=1.0 && LightScatteringPosition.x<UV.x)
			l_LightScatteringPosition=float2(0.0, l_LightScatteringPositionLeft);
		else if(l_LightScatteringPositionTop>=0 && l_LightScatteringPositionTop<=1.0 && LightScatteringPosition.y<UV.y)
			l_LightScatteringPosition=float2(l_LightScatteringPositionTop, 0.0);
		else if(l_LightScatteringPositionBottom>=0 && l_LightScatteringPositionBottom<=1.0 && LightScatteringPosition.y>=UV.y)
			l_LightScatteringPosition=float2(l_LightScatteringPositionBottom, 1.0);
	}
	return l_LightScatteringPosition;
}

float4 PS(PS_INPUT IN) : SV_Target
{
	if(!m_Active)
		return float4(0,0,0,0);
	float2 l_LightScatteringPosition=ClampLightScatteringPosition(IN.UV,m_LightScatteringPosition);
	float2 l_DeltaTextCoord=IN.UV-l_LightScatteringPosition;
	float2 l_TextCoo=IN.UV;
	l_DeltaTextCoord *= 1.0/float(m_NumSamples) * m_Density;
	float l_IlluminationDecay=1.0;
	float4 l_Color=float4(0,0,0,0);
	for(int i=0; i < m_NumSamples ; i++)
	{
		l_TextCoo -= l_DeltaTextCoord;
		float4 l_Sample=T0Texture.SampleGrad(S0Sampler, l_TextCoo, 0, 0);
		l_Sample*=l_IlluminationDecay*m_Weight;
		l_Color+=l_Sample;
		l_IlluminationDecay*=m_Decay;
	}
	return float4(l_Color.xyz*m_Exposure, 0.0);
}