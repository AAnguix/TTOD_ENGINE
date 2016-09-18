#include "globals.fxh"
#include "samplers.fxh"
#include "functions.fxh"

static float m_SpecularFactor=m_RawDataValues[0];
static float m_Gloss=m_RawDataValues[1];
static float m_BumpFactor=m_RawDataValues[2];
static float n_EnvironmentFactor=m_RawDataValues[3];
static float m_CutOut=m_RawDataValues[4];
static float m_SsrFactor=m_RawDataValues[5];

static float3 m_Scales = float3(1.0,2.0, 3.0);
static float3 m_ScrollSpeeds = float3(0.2,0.6, 1.3);
static float2 m_Distortion1 = float2(0.1,0.2);
static float2 m_Distortion2 = float2(0.2,0.3);
static float2 m_Distortion3 = float2(0.3,0.1);
static float m_DistortionScale = 0.8;
static float m_DistortionBias = 0.5;

struct VS_INPUT
{
	float3 Pos : POSITION;
	float3 Normal : NORMAL;
	float2 UV : TEXCOORD0;
	float4 Weight : BLENDWEIGHT;
	float4 Indices : BLENDINDICES;
};

struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float2 UV : TEXCOORD0;
	float3 Normal : TEXCOORD1;
	float4 HPos : TEXCOORD2;
	float3 WorldPos : TEXCOORD3;
	float2 UV1 : TEXCOORD4;
	float2 UV2 : TEXCOORD5;
	float2 UV3 : TEXCOORD6;
};

struct PixelOutputType
{
	float4 Target0 : SV_Target0; //Albedo (float3) + (float) SpecularFactor
	float4 Target1 : SV_Target1; //AmbientLight (float3) + (float) SpecularPow
	float4 Target2 : SV_Target2; //Normal (float3) + (float) Not used
	float4 Target3 : SV_Target3; //Depth (float4)
};


PS_INPUT VS( VS_INPUT IN )
{
	PS_INPUT l_Output = (PS_INPUT)0;
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
	l_Output.HPos = l_Output.Pos;
	l_Output.Normal=normalize(mul(normalize(l_Normal).xyz, (float3x3)m_World));
	l_Output.UV = IN.UV;
	
	
	//l_Output.UV1 =IN.UV;
	//l_Output.UV2 =IN.UV;
	//l_Output.UV3 =IN.UV;
	
	
	l_Output.UV1 = IN.UV*m_Scales.x;
	l_Output.UV1.y = l_Output.UV1.y+(m_Times.y*m_ScrollSpeeds.x);
	l_Output.UV2 = IN.UV*m_Scales.y;
	l_Output.UV2.y = l_Output.UV2.y+(m_Times.y*m_ScrollSpeeds.y);
	l_Output.UV3 = IN.UV*m_Scales.z;
	l_Output.UV3.y = l_Output.UV3.y+(m_Times.y*m_ScrollSpeeds.z);
	
	return l_Output;
}


PixelOutputType PS( PS_INPUT IN) : SV_Target
{
	PixelOutputType l_Output;
	

	// Sample the same noise texture using the three different texture coordinates to get three different noise scales.
	float4 noise1;
	float4 noise2;
	float4 noise3;
	float4 finalNoise;
	float4 fireColor;
	float4 alphaColor;
	float2 noiseCoords;
	float perturb;
	
	noise1 = T4Texture.Sample(S1Sampler, IN.UV1);
    noise2 = T4Texture.Sample(S1Sampler, IN.UV2);
    noise3 = T4Texture.Sample(S1Sampler, IN.UV3);
	
	// Move the noise from the (0, 1) range to the (-1, +1) range.
    noise1 = (noise1 - 0.5f) * 2.0f;
    noise2 = (noise2 - 0.5f) * 2.0f;
    noise3 = (noise3 - 0.5f) * 2.0f;
	
	// Distort the three noise x and y coordinates by the three different distortion x and y values.
    noise1.xy = noise1.xy * m_Distortion1.xy;
    noise2.xy = noise2.xy * m_Distortion2.xy;
    noise3.xy = noise3.xy * m_Distortion3.xy;

    // Combine all three distorted noise results into a single noise result.
    finalNoise = noise1 + noise2 + noise3;
	
	 // Perturb the input texture Y coordinates by the distortion scale and bias values.  
    // The perturbation gets stronger as you move up the texture which creates the flame flickering at the top effect.
    perturb = ((1.0f - IN.UV.y) * m_DistortionScale) + m_DistortionBias;

    // Now create the perturbed and distorted texture sampling coordinates that will be used to sample the fire color texture.
    noiseCoords.xy = (finalNoise.xy * perturb) + IN.UV.xy;
	//noiseCoords.xy = finalNoise.xy + IN.UV.xy;
	  // Sample the color from the fire texture using the perturbed and distorted texture sampling coordinates.
    // Use the clamping sample state instead of the wrap sample state to prevent flames wrapping around.
    fireColor = T3Texture.Sample(S2Sampler, noiseCoords.xy);
	
    // Sample the alpha value from the alpha texture using the perturbed and distorted texture sampling coordinates.
    // This will be used for transparency of the fire.
    // Use the clamping sample state instead of the wrap sample state to prevent flames wrapping around.
    //alphaColor = T1Texture.Sample(S2Sampler, noiseCoords.xy);
	alphaColor = T1Texture.Sample(S2Sampler, IN.UV);
	
	float4 l_Base = T0Texture.Sample(S0Sampler, IN.UV);
	//float4 l_Diffuse = T1Texture.Sample(S0Sampler, IN.UV);
	
	//float4 l_Diffuse = l_Base + fireColor;
	//float4 l_Diffuse = (alphaColor*fireColor)+l_Base;
	//float4 l_Diffuse = l_Base + (alphaColor*fireColor);
	//float4 l_Diffuse = fireColor; // l_Base+fireColor 0 l_Base*fireColor VER CUAL QUEDA MEJOR
	//float4 l_Diffuse = fireColor*l_Base ;
	//float4 l_Diffuse = fireColor;
	float4 l_Diffuse = l_Base ;
	//l_Diffuse.a = alphaColor;
	if(l_Diffuse.a<m_CutOut)
		clip(-1);
		
	float l_Spec = 0.0;
		
	l_Output.Target0.xyz = l_Diffuse.xyz;
	
	float3 l_Target1 = float3(0.0,0.0,0.0);
	
	


	l_Output.Target0.xyz = l_Diffuse.xyz;
	l_Target1 = (m_LightAmbient.xyz*0.0)*l_Output.Target0.xyz;
	l_Output.Target2 = float4(Normal2Texture(IN.Normal.xyz), 1.0f);
	
	l_Output.Target1.xyz = l_Target1;
	
	l_Output.Target0.w = m_SpecularFactor * (1-l_Spec);
	l_Output.Target1.w = 1/m_Gloss;
	
	l_Output.Target2.w = m_SsrFactor;
	
	float l_Depth = IN.HPos.z / IN.HPos.w;
	l_Output.Target3=float4(l_Depth, l_Depth, l_Depth, 1.0);
	return l_Output;
	
}
