#include "globals.fxh"
#include "LightFunctions.fxh"
#include "samplers.fxh"

static float m_SpecularFactor=m_RawDataValues[0];
static float m_Gloss=m_RawDataValues[1];
static float m_BumpFactor=m_RawDataValues[2];
static float n_EnvironmentFactor=m_RawDataValues[3];
static float m_CutOut=m_RawDataValues[4];

struct VS_INPUT
{
	float3 Pos : POSITION;

	#ifdef HAS_WEIGHTS
		float4 Weight : BLENDWEIGHT;
		float4 Indices : BLENDINDICES;
	#endif	

	float3 Normal : NORMAL;

	#ifdef HAS_NORMAL
		float4 Tangent : TANGENT;
		float4 Binormal : BINORMAL;
	#endif

	float2 UV : TEXCOORD0;
};
struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float2 UV : TEXCOORD0;
	float3 Normal : TEXCOORD1;
	float3 WorldPos: TEXCOORD2;
	#ifdef HAS_NORMAL
		float3 WorldTangent : TEXCOORD3;
		float3 WorldBinormal : TEXCOORD4;
	#endif 
};
PS_INPUT VS( VS_INPUT IN )
{
	PS_INPUT l_Output = (PS_INPUT)0;

	#ifdef HAS_WEIGHTS
		float4 l_TempPos=float4(IN.Pos.xyz, 1.0);
		float3 l_Position = float3(0.0,0.0,0.0);
		float3 l_Normal = float3(0.0,0.0,0.0);
		float4 l_Indices=IN.Indices;
		
		l_Position=mul(l_TempPos, m_Bones[l_Indices.x]) * IN.Weight.x;
		l_Position+=mul(l_TempPos, m_Bones[l_Indices.y]) * IN.Weight.y;
		l_Position+=mul(l_TempPos, m_Bones[l_Indices.z]) * IN.Weight.z;
		l_Position+=mul(l_TempPos, m_Bones[l_Indices.w]) * IN.Weight.w;
		
		float3x3 m;

		m[0].xyz = m_Bones[l_Indices.x][0].xyz;
		m[1].xyz = m_Bones[l_Indices.x][1].xyz;
		m[2].xyz = m_Bones[l_Indices.x][2].xyz;

		l_Normal+=mul(IN.Normal.xyz, m)* IN.Weight.x;

		m[0].xyz = m_Bones[l_Indices.y][0].xyz;
		m[1].xyz = m_Bones[l_Indices.y][1].xyz;
		m[2].xyz = m_Bones[l_Indices.y][2].xyz;

		l_Normal+=mul(IN.Normal.xyz, m)* IN.Weight.y;
		
		l_Normal=normalize(l_Normal);

		l_Output.Pos = mul(float4(l_Position, 1.0), m_World);
		l_Output.WorldPos = mul(IN.Pos, m_World).xyz;
		l_Output.Pos = mul(l_Output.Pos, m_View);
		l_Output.Pos = mul(l_Output.Pos, m_Projection);
		
		l_Output.Normal=normalize(mul(normalize(l_Normal).xyz, (float3x3)m_World));	
	#else
		l_Output.Pos = mul( float4(IN.Pos.xyz,1.0), m_World ); 
		l_Output.WorldPos=l_Output.Pos.xyz;
		l_Output.Pos = mul( l_Output.Pos, m_View );
		l_Output.Pos = mul( l_Output.Pos, m_Projection );
		
		l_Output.Normal = normalize(mul(normalize(IN.Normal).xyz, (float3x3)m_World));
	#endif
	
	l_Output.UV = IN.UV;
	
	#ifdef HAS_NORMAL
		l_Output.WorldTangent = mul(IN.Tangent.xyz,(float3x3)m_World);
		l_Output.WorldBinormal = mul(cross(IN.Tangent.xyz,IN.Normal),(float3x3)m_World);
	#endif
	
	return l_Output;
}
float4 PS( PS_INPUT IN) : SV_Target
{
	float4 l_Diffuse=T0Texture.Sample(S0Sampler, IN.UV);
	
	if(l_Diffuse.a<m_CutOut)
		clip(-1);
		
	float l_Spec = 0.0;
	
	float3 Nn=IN.Normal;
	
	#ifdef HAS_NORMAL
		float4 l_NormalMapTexture = T1Texture.Sample(S1Sampler, IN.UV);
		l_Spec = l_NormalMapTexture.a;
		float3 l_TangentNormalized=normalize(IN.WorldTangent);
		float3 l_BinormalNormalized=normalize(IN.WorldBinormal); 
		float3 l_Bump=m_BumpFactor*(l_NormalMapTexture.rgb - float3(0.5,0.5,0.5));	
		Nn = normalize(IN.Normal);
		Nn = Nn + (l_Bump.x*l_TangentNormalized) + (l_Bump.y*l_BinormalNormalized);
	#endif
	
	m_SpecularFactor = m_SpecularFactor*(1-l_Spec);
	
	float3 l_PixelColor = GetIluminatedPixelColor(l_Diffuse,m_SpecularFactor,m_Gloss,IN.WorldPos,Nn, m_LightAmbient, MAX_LIGHTS_BY_SHADER);
	return float4(l_PixelColor,l_Diffuse.a);
}