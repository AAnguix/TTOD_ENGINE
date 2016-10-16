#include "globals.fxh"
#include "functions.fxh"
#include "samplers.fxh"

static float m_SpecularFactor=m_RawDataValues[0];
static float m_Gloss=m_RawDataValues[1];
static float m_BumpFactor=m_RawDataValues[2];
static float n_EnvironmentFactor=m_RawDataValues[3];
static float m_CutOut=m_RawDataValues[4];
static float m_SsrFactor=m_RawDataValues[5];

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
	#ifdef HAS_LIGHTMAP
		float2 UV2: TEXCOORD1;
	#endif
	
	//RNM
	#ifdef HAS_RNM 
		float2 UV2: TEXCOORD1;
	#endif
};
struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float2 UV : TEXCOORD0;
	
	#ifdef HAS_LIGHTMAP 
		float2 UV2: TEXCOORD1;
		float3 Normal : TEXCOORD2;
		float4 HPos : TEXCOORD3;
		float3 WorldPos : TEXCOORD4;
	#elif HAS_RNM
		float2 UV2: TEXCOORD1;
		float3 Normal : TEXCOORD2;
		float4 HPos : TEXCOORD3;
		float3 WorldPos : TEXCOORD4;	
	#else
		float3 Normal : TEXCOORD1;
		float4 HPos : TEXCOORD2;
		float3 WorldPos : TEXCOORD3;
	#endif
	
	#ifdef HAS_NORMAL
		//RNM
		#ifdef HAS_RNM 
			float3 WorldTangent : TEXCOORD5;
			float3 WorldBinormal : TEXCOORD6;
		#elif HAS_LIGHTMAP 
			float3 WorldTangent : TEXCOORD5;
			float3 WorldBinormal : TEXCOORD6;
		#else
			float3 WorldTangent : TEXCOORD4;
			float3 WorldBinormal : TEXCOORD5;
		#endif
		
	#endif
	
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
		l_Output.HPos = l_Output.Pos;
		l_Output.Normal=normalize(mul(normalize(l_Normal).xyz, (float3x3)m_World));	

	#else
		float4 l_Pos=mul(float4(IN.Pos,1.0), m_World );
		l_Output.WorldPos = l_Pos.xyz;
		float4 l_HPos=mul(l_Pos, m_View);
		l_HPos=mul(l_HPos, m_Projection);
		l_Output.Pos=l_HPos;
		l_Output.HPos = l_HPos; //Projección
		
		l_Output.Normal = normalize(mul(normalize(IN.Normal).xyz, (float3x3)m_World));
	#endif
	
	l_Output.UV = IN.UV;
	#ifdef HAS_LIGHTMAP
		l_Output.UV2 = IN.UV2;
	#endif
	
	#ifdef HAS_RNM 
		l_Output.UV2 = IN.UV2;
	#endif
	
	#ifdef HAS_NORMAL
		l_Output.WorldTangent = mul(IN.Tangent.xyz,(float3x3)m_World);
		l_Output.WorldBinormal = mul(cross(IN.Tangent.xyz,IN.Normal),(float3x3)m_World);
	#endif
	
	return l_Output;
}

PixelOutputType PS( PS_INPUT IN) : SV_Target
{
	PixelOutputType l_Output;

	float4 l_Diffuse = T0Texture.Sample(S0Sampler, IN.UV);
	float l_Noise=float4(0.0,0.0,0.0,0.0);
		
	float l_Spec = 0.0;
		
	l_Output.Target0.xyz = l_Diffuse.xyz;
	
	float3 l_Target1 = float3(0.0,0.0,0.0);
	
	float3 l_ReflectColor=float3(0.0,0.0,0.0);
	
	#ifdef HAS_ENVIRONMENT
			float3 Nn = normalize(IN.Normal);
		#ifdef HAS_NORMAL
			float4 l_NormalMapTexture = T1Texture.Sample(S1Sampler, IN.UV);
			l_Spec = l_NormalMapTexture.a;
			float3 l_TangentNormalized=normalize(IN.WorldTangent);
			float3 l_BinormalNormalized=normalize(IN.WorldBinormal); 
			float3 l_Bump=m_BumpFactor*(l_NormalMapTexture.rgb - float3(0.5,0.5,0.5));	
			Nn = Nn + (l_Bump.x*l_TangentNormalized) + (l_Bump.y*l_BinormalNormalized);
			l_Output.Target2 = float4(Normal2Texture(Nn), 1.0f); //Bumpmap
			float3 l_EyeToWorldPosition = normalize(IN.WorldPos - m_InverseView[3].xyz);
			float3 l_ReflectVector = normalize(reflect(l_EyeToWorldPosition, Nn));
			l_ReflectColor = T2CubeTexture.Sample(S2Sampler, l_ReflectVector).xyz;
			l_Noise = T3Texture.Sample(S3Sampler, IN.UV);
		#else 
			float3 l_EyeToWorldPosition = normalize(IN.WorldPos - m_InverseView[3].xyz);
			float3 l_ReflectVector = normalize(reflect(l_EyeToWorldPosition, Nn));
			l_ReflectColor = T1CubeTexture.Sample(S1Sampler, l_ReflectVector).xyz;
			l_Noise = T2Texture.Sample(S2Sampler, IN.UV);
			l_Output.Target2 = float4(Normal2Texture(IN.Normal.xyz), 1.0f); //Alpha not used
		#endif
			
			l_Target1 = m_LightAmbient.xyz*l_Output.Target0.xyz + (l_ReflectColor*n_EnvironmentFactor);
	#else
		#ifdef HAS_LIGHTMAP
			// float4 Color1 = float4(1.0,0.0,0.0,1.0);
			//l_Output.Target0.xyz = Color1.xyz;
			float4 l_LightMap=T1Texture.Sample(S1Sampler, IN.UV2);
			l_Target1 = l_LightMap*l_Output.Target0.xyz;
			#ifdef HAS_NORMAL
				float4 l_NormalMapTexture = T2Texture.Sample(S2Sampler, IN.UV);
				l_Noise = T3Texture.Sample(S3Sampler, IN.UV);
				l_Spec = l_NormalMapTexture.a;
				float3 l_TangentNormalized=normalize(IN.WorldTangent);
				float3 l_BinormalNormalized=normalize(IN.WorldBinormal); 
				float3 l_Bump=m_BumpFactor*(l_NormalMapTexture.rgb - float3(0.5,0.5,0.5));	
				float3 Nn = normalize(IN.Normal);
				Nn = Nn + (l_Bump.x*l_TangentNormalized) + (l_Bump.y*l_BinormalNormalized);
				l_Output.Target2 = float4(Normal2Texture(Nn), 1.0f); 
			#else 
				l_Noise = T2Texture.Sample(S2Sampler, IN.UV);
				l_Output.Target2 = float4(Normal2Texture(IN.Normal.xyz), 1.0f);
			#endif
		#else //NO LIGHTMAP
			l_Target1 = m_LightAmbient.xyz*l_Output.Target0.xyz; 
			#ifdef HAS_NORMAL
				
				#ifdef HAS_RNM
					float4 l_NormalMapTexture = T4Texture.Sample(S4Sampler, IN.UV);
					l_Noise = T5Texture.Sample(S5Sampler, IN.UV);
				#else
					float4 l_NormalMapTexture = T1Texture.Sample(S1Sampler, IN.UV);
					l_Noise = T2Texture.Sample(S2Sampler, IN.UV);
				#endif
				
				l_Spec = l_NormalMapTexture.a;
				float3 l_TangentNormalized=normalize(IN.WorldTangent);
				float3 l_BinormalNormalized=normalize(IN.WorldBinormal); 
				float3 l_Bump=m_BumpFactor*(l_NormalMapTexture.rgb - float3(0.5,0.5,0.5));	
				float3 Nn = normalize(IN.Normal);
				Nn = Nn + (l_Bump.x*l_TangentNormalized) + (l_Bump.y*l_BinormalNormalized);
				l_Output.Target2 = float4(Normal2Texture(Nn), 1.0f); 
				#ifdef HAS_RNM 
					//Nn=CalcNormalMap(Nn, Tn, Bn, l_NormalMap);
					float3 L_RNM = GetRadiosityNormalMap(Nn, IN.UV2, T1Texture, S1Sampler,  T2Texture, S2Sampler, T3Texture, S3Sampler);
					L_RNM=T1Texture.Sample(S1Sampler, IN.UV2).xyz;
					l_Target1 = L_RNM*l_Output.Target0.xyz;
				#endif
			#else 
				l_Output.Target2 = float4(Normal2Texture(IN.Normal.xyz), 1.0f);
				l_Noise = T1Texture.Sample(S1Sampler, IN.UV);
			#endif
		#endif
	#endif
	
	l_Output.Target1.xyz = l_Target1;
	
	l_Output.Target0.w = m_SpecularFactor * (1-l_Spec);
	l_Output.Target1.w = 1/m_Gloss;
	
	l_Output.Target2.w = m_SsrFactor;
	
	float l_Depth = IN.HPos.z / IN.HPos.w;
	l_Output.Target3=float4(l_Depth, l_Depth, l_Depth, 1.0);
	
	if(l_Noise.r<m_CutOut)
		clip(-1);
	
	return l_Output;
}