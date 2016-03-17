#include "globals.fxh"
#include "LightFunctions.fxh"
#include "samplers.fxh"

struct VS_INPUT
{
	float3 Pos : POSITION;
	float3 Normal : NORMAL;
	#ifdef HAS_NPORMAL
		float4 Tangent : TANGENT;
		float4 Binormal : BINORMAL;
	#endif
	float2 UV : TEXCOORD0;
};
struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float2 UV : TEXCOORD0;
	float3 WorldNormal : TEXCOORD1;
	float3 WorldPos: TEXCOORD2;
	#ifdef HAS_NPORMAL
		float3 WorldTangent : TEXCOORD3;
		float3 WorldBinormal : TEXCOORD4;
	#endif 
};
PS_INPUT VS( VS_INPUT IN )
{
	PS_INPUT l_Output = (PS_INPUT)0;
	l_Output.Pos = mul( float4(IN.Pos.xyz,1.0), m_World ); 
	l_Output.WorldPos=l_Output.Pos.xyz;
	l_Output.Pos = mul( l_Output.Pos, m_View );
	l_Output.Pos = mul( l_Output.Pos, m_Projection );
	l_Output.UV = IN.UV;
	l_Output.WorldNormal = normalize(mul(normalize(IN.Normal).xyz, (float3x3)m_World));
	#ifdef HAS_NORMAL
		l_Output.WorldTangent = mul(IN.Tangent.xyz,(float3x3)m_World);
		l_Output.WorldBinormal = mul(cross(IN.Tangent.xyz,IN.Normal),(float3x3)m_World);
	#endif
	return l_Output;
}
float4 PS( PS_INPUT IN) : SV_Target
{
	float4 l_Texture=T0Texture.Sample(S0Sampler, IN.UV);
	float l_SpecularPower=60.0f;
	float3 Nn=IN.WorldNormal;
	
	#ifdef HAS_NORMAL
		float l_Depht=2.4f;
		float4 l_NormalMapTexture = T1Texture.Sample(S1Sampler, IN.UV);
		float3 l_TangentNormalized=normalize(IN.WorldTangent);
		float3 l_BinormalNormalized=normalize(IN.WorldBinormal); 
		float3 l_Bump=l_Depht*(l_NormalMapTexture.rgb - float3(0.5,0.5,0.5));	
		Nn = normalize(IN.WorldNormal);
		Nn = Nn + (l_Bump.x*l_TangentNormalized) + (l_Bump.y*l_BinormalNormalized);
	#endif
	
	float3 l_PixelColor = GetIluminatedPixelColor(l_Texture,l_SpecularPower,IN.WorldPos,Nn, m_LightAmbient, MAX_LIGHTS_BY_SHADER);
	return float4(l_PixelColor,l_Texture.a);
}