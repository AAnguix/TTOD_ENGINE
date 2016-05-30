#include "globals.fxh"
#include "LightFunctions.fxh"
#include "samplers.fxh"

static float m_SpecularFactor=m_RawDataValues[0];
static float m_Gloss=m_RawDataValues[1];
static float m_BumpFactor=m_RawDataValues[2];
static float n_EnvironmentFactor=m_RawDataValues[3];
static float m_CutOut=m_RawDataValues[4];

//Lava parameters
static float m_Smooth = 8.0f;
static float m_LavaSpeed1 = 0.02f;
static float l_DiffuseSpeed = 0.01f;
static float m_MoveVertices = true;

static float m_GranulatedX = 0.2f;
static float m_GranulatedY = 0.2f;

struct VS_INPUT
{
	float3 Pos : POSITION;
	float3 Normal : NORMAL;
	float4 Tangent : TANGENT;
	float4 Binormal : BINORMAL;
	float2 UV : TEXCOORD0;
};
struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float2 UV : TEXCOORD0;
	float3 Normal : TEXCOORD1;
	float3 WorldPos: TEXCOORD2;
	float3 WorldTangent : TEXCOORD3;
	float3 WorldBinormal : TEXCOORD4;

};

PS_INPUT VS( VS_INPUT IN )
{
	float l_Random = m_Times.z;
	float l_Smooth = 8.0;
	PS_INPUT l_Output = (PS_INPUT)0;
	l_Output.Pos = mul( float4(IN.Pos.xyz,1.0), m_World ); 
	l_Output.WorldPos=l_Output.Pos.xyz;
	l_Output.Pos = mul( l_Output.Pos, m_View );
	l_Output.Pos = mul( l_Output.Pos, m_Projection );
	
	if(m_MoveVertices)
	{
		float2 l_UV = IN.UV;
		float l_Factor = (l_UV.x*l_UV.y) /2;
		l_Factor = sqrt(l_Factor);
		float l_Value = sin((l_UV.x)*m_Times.y) + cos((l_UV.y)*m_Times.y);
		float l_Displacement = (l_Value/m_Smooth);
		l_Output.Pos.y += l_Displacement;
	}
	
	// EQ1 l_Output.Pos.y += ( (sin(m_Times.y)/4)*l.x) + ( (cos(m_Times.y)/4)*/l.y );
	// EQ2 l_Output.Pos.y += ( (sin(m_Times.y)/4)*(0.1/l.x)) + ( (cos(m_Times.y)/4)*(0.1/l.y ));
	
	//FUNCIONA
	// float l_Factor = (l_UV.x*l_UV.y) /2;
	// l_Factor = 1/l_Factor;
	// float l_Displacement = (sin((l_UV.x)*m_Times.y) - cos((l_UV.y)*m_Times.y))/l_Smooth;
	// l_Output.Pos.y += l_Displacement;
	
	
	//l_Output.Pos.x += sin(m_Times.y)/8;
	l_Output.Normal = normalize(mul(normalize(IN.Normal).xyz, (float3x3)m_World));
	
	l_Output.UV = IN.UV;
	
	l_Output.WorldTangent = mul(IN.Tangent.xyz,(float3x3)m_World);
	l_Output.WorldBinormal = mul(cross(IN.Tangent.xyz,IN.Normal),(float3x3)m_World);

	return l_Output;
}
//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS( PS_INPUT IN) : SV_Target
{
	float3 Nn=IN.Normal;
	float l_Spec = 0.0;
	
	float4 l_NormalMapTexture = T2Texture.Sample(S2Sampler, IN.UV);
	l_Spec = l_NormalMapTexture.a;
	float3 l_TangentNormalized=normalize(IN.WorldTangent);
	float3 l_BinormalNormalized=normalize(IN.WorldBinormal); 
	float3 l_Bump=m_BumpFactor*(l_NormalMapTexture.rgb - float3(0.5,0.5,0.5));	
	Nn = normalize(IN.Normal);
	Nn = Nn + (l_Bump.x*l_TangentNormalized) + (l_Bump.y*l_BinormalNormalized);

	float2 l_DiffuseUV = IN.UV + float2(-0.5,2.0)*m_Times.y*l_DiffuseSpeed; 
	float2 l_NoiseUV = IN.UV + float2(1.5,-1.5)*m_Times.y*m_LavaSpeed1; 
   
	float4 l_NoiseColor = T1Texture.Sample(S1Sampler, IN.UV);
	l_NoiseUV.x += (l_NoiseColor.x)*2.0; 
	l_NoiseUV.y += (l_NoiseColor.y)*2.0; 
	l_DiffuseUV.x += (l_NoiseColor.y)*m_GranulatedX; 
	l_DiffuseUV.y += (l_NoiseColor.z)*m_GranulatedY; 

	float l_Noise = T1Texture.Sample( S1Sampler, l_NoiseUV*2.0).a;
	
	float4 l_Color = T0Texture.Sample(S0Sampler, l_DiffuseUV*2.0); 
	
	float4 l_Diffuse = l_Color*(float4(l_Noise,l_Noise,l_Noise,l_Noise)*2.0)+(l_Color*l_Color-0.1); 
	
	if(l_Diffuse.r > 1.0 ) { l_Diffuse.bg += clamp(l_Diffuse.r-2.0,0.0,100.0); } 
	if(l_Diffuse.g > 1.0 ) { l_Diffuse.rb += l_Diffuse.g-1.0; } 
	if(l_Diffuse.b > 1.0 ) { l_Diffuse.rg += l_Diffuse.b-1.0; } 
	
	m_SpecularFactor = m_SpecularFactor*(1-l_Spec);
	
	float3 l_PixelColor = GetIluminatedPixelColor(l_Diffuse,m_SpecularFactor,m_Gloss,IN.WorldPos,Nn, m_LightAmbient, MAX_LIGHTS_BY_SHADER);
	return float4(l_PixelColor,l_Diffuse.a);
}

float GetVertexLavaPosition(float4 Position)
{
	return Position;
}