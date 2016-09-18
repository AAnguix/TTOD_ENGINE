#include "globals.fxh"
#include "samplers.fxh"

static float3 m_StartPosition=float3(m_RawDataValues[0], m_RawDataValues[1], m_RawDataValues[2]);
static float3 m_EndPosition=float3(m_RawDataValues[3], m_RawDataValues[4], m_RawDataValues[5]);
// static float2 m_Intensity = float(m_RawDataValues[6])

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
	float3 Normal : TEXCOORD1;
	float3 WorldPos: TEXCOORD2;
};

PS_INPUT VS( VS_INPUT IN )
{
	PS_INPUT l_Output = (PS_INPUT)0;
	
	// float4x4 m_ModelView = mul(m_World,m_View);

	// m_ModelView[0][0] = 1.0; 
	// m_ModelView[0][2] = 0.0; 
	
	// m_ModelView[2][0] = 0.0; 
	// m_ModelView[2][2] = 1.0; 
	
	// l_Output.Pos = mul(float4(IN.Pos.xyz,1.0), m_World);
	// l_Output.Pos = mul(float4(IN.Pos.xyz,1.0), m_View);
	// l_Output.Pos = mul(l_Output.Pos, m_Projection );
	
	// l_Output.Normal = normalize(mul(normalize(IN.Normal).xyz, (float3x3)m_World));
	// l_Output.UV = IN.UV;
	// l_Output.WorldTangent = mul(IN.Tangent.xyz,(float3x3)m_World);
	// l_Output.WorldBinormal = mul(cross(IN.Tangent.xyz,IN.Normal),(float3x3)m_World);
	
	float4 l_Pos=mul(float4(IN.Pos,1.0), m_World );
	l_Output.WorldPos = l_Pos.xyz;
	float4 l_HPos=mul(l_Pos, m_View);
	l_HPos=mul(l_HPos, m_Projection);
	l_Output.Pos=l_HPos;
	
	return l_Output;
}

float4 PS( PS_INPUT IN) : SV_Target
{
	return float4(1.0,0.0,0.0,1.0);
}



// PS_INPUT VS( VS_INPUT IN )
// {
	// PS_INPUT l_Output = (PS_INPUT)0;
	
	// float4x4 m_ModelView = mul(m_World,m_View);

	// m_ModelView[0][0] = 1.0; 
	// m_ModelView[0][2] = 0.0; 
	
	// m_ModelView[2][0] = 0.0; 
	// m_ModelView[2][2] = 1.0; 
	
	// l_Output.Pos = mul(float4(IN.Pos.xyz,1.0), m_ModelView);
	// l_Output.Pos = mul(l_Output.Pos, m_Projection );
	
	// l_Output.Normal = normalize(mul(normalize(IN.Normal).xyz, (float3x3)m_World));
	// l_Output.UV = IN.UV;
	
	// return l_Output;
// }

// float4 PS( PS_INPUT IN) : SV_Target
// {
	// float4 l_Diffuse = T0Texture.Sample(S0Sampler, IN.UV);
	// return l_Diffuse;
	// return float4(1.0,0.0,0.0,1.0);
// }


