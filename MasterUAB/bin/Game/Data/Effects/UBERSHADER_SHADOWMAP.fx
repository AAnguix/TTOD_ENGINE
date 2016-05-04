/*Used to generate the shadowmap*/

#include "globals.fxh"
#include "functions.fxh"
#include "samplers.fxh"

//--------------------------------------------------------------------------------------
struct VS_INPUT
{
	float3 Pos : POSITION;
	#ifdef HAS_WEIGHTS
		float4 Weight : BLENDWEIGHT;
		float4 Indices : BLENDINDICES;
	#endif
};

struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float4 Depth : TEXCOORD0;
};

PS_INPUT VS(VS_INPUT IN)
{
	PS_INPUT l_Out=(PS_INPUT)0;
	
	#ifndef HAS_WEIGHTS
		l_Out.Pos=mul(float4(IN.Pos.xyz, 1.0), m_World);
		l_Out.Pos=mul(l_Out.Pos, m_View);
		l_Out.Pos=mul(l_Out.Pos, m_Projection);
	#else
		float4 l_TempPos=float4(IN.Pos.xyz, 1.0);
		float3 l_Position= 0;
		float4 l_Indices=IN.Indices;
		
		l_Position=mul(l_TempPos, m_Bones[l_Indices.x]) * IN.Weight.x;
		l_Position+=mul(l_TempPos, m_Bones[l_Indices.y]) * IN.Weight.y;
		l_Position+=mul(l_TempPos, m_Bones[l_Indices.z]) * IN.Weight.z;
		l_Position+=mul(l_TempPos, m_Bones[l_Indices.w]) * IN.Weight.w;

		l_Out.Pos = mul(float4(l_Position, 1.0), m_World);
		l_Out.Pos = mul(l_Out.Pos, m_View);
		l_Out.Pos = mul(l_Out.Pos, m_Projection);
	#endif
	
	l_Out.Depth=l_Out.Pos;
	return l_Out;
}

float4 PS(PS_INPUT IN) : SV_Target
{
	float l_Depth=IN.Depth.z/IN.Depth.w;
	return float4(l_Depth,l_Depth,l_Depth,1);
	//Distance from pixel to the light
}