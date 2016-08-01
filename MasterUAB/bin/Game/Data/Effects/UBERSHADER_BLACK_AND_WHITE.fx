#include "globals.fxh"
#include "functions.fxh"
#include "LightFunctions.fxh"
#include "samplers.fxh"

struct VS_INPUT
{
	float4 Pos : POSITION;
	
	#ifdef HAS_WEIGHTS
		float4 Weight : BLENDWEIGHT;
		float4 Indices : BLENDINDICES;
	#endif
	
	float2 UV : TEXCOORD0;
};
struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float2 UV : TEXCOORD0;
};
PS_INPUT VS(VS_INPUT IN)
{
	PS_INPUT l_Output = (PS_INPUT)0;
	l_Output.Pos=IN.Pos;
	l_Output.UV=IN.UV;
	return l_Output;
}

float GetLightInt(int IdLight, float3 WorldPos, float3 Nn)
{
	if(m_LightEnabledArray[IdLight]) //Enabled
	{
		if(m_LightTypeArray[IdLight]==0.0f) //OMNI
		{
			float3 lightDirection = m_LightPosition[IdLight].xyz - WorldPos; //Punto final - Punto inicio
			float l_Distance=length(lightDirection);
			float l_Attenuation =  GetAttenuation( l_Distance, m_LightAttenuationStartRangeArray[IdLight], m_LightAttenuationEndRangeArray[IdLight]);
			lightDirection/=l_Distance; //Normalize
			
			float l_DiffuseContrib = GetOmniDiffuseContrib(Nn,lightDirection);
			return (l_DiffuseContrib * m_LightIntensityArray[IdLight] * l_Attenuation);
		}
		else if(m_LightTypeArray[IdLight]==1.0f) //DIRECTIONAL
		{
			float l_DiffuseContrib = GetDirectionalDiffuseContrib(Nn,m_LightDirection[IdLight]);
			return (l_DiffuseContrib * m_LightIntensityArray[IdLight]);
		}
		else if(m_LightTypeArray[IdLight]==2.0f) //SPOT
		{
			float3 lightDirection = m_LightPosition[IdLight].xyz - WorldPos; //Punto final - Punto inicio
			float l_Distance=length(lightDirection);
			float l_Attenuation =  GetAttenuation( l_Distance, m_LightAttenuationStartRangeArray[IdLight], m_LightAttenuationEndRangeArray[IdLight]);
			lightDirection/=l_Distance; //Normalize
			
			float l_SpotAttenuation =  GetSpotAttenuation(m_LightAngleArray[IdLight],m_LightFallOffAngleArray[IdLight],lightDirection,m_LightDirection[IdLight]);
			
			float l_DiffuseContrib = GetSpotDiffuseContrib(Nn,lightDirection);
			return (l_DiffuseContrib * m_LightIntensityArray[IdLight] * l_Attenuation * l_SpotAttenuation);
		}
		
		return 0.0;
	}
	
	return 0.0;
}

float4 PS(PS_INPUT IN) : SV_Target
{
	float4 l_NormalMap = T0Texture.Sample(S1Sampler, IN.UV);
	float l_Depth = T1Texture.Sample(S2Sampler, IN.UV).r;
	
	float l_SpecularPower=60.0f;
	
	float3 l_WorldPos=GetPositionFromZDepthView(l_Depth, IN.UV,m_InverseView, m_InverseProjection);
	float3 l_WorldNormal = normalize(Texture2Normal(l_NormalMap.xyz));
	
	float3 l_DiffuseLight;
	float3 l_SpecularLight;
	
	float4 color = float4(0.0f, 0.0f, 0.0f, 1.0f);
	float l_Bias=0.001;
	float l_LightIntensity;
	
	if(m_UseShadowMapArray[0])
	{	
		float4 l_LightViewPosition=mul(float4(l_WorldPos, 1), m_LightView[0]);
		l_LightViewPosition=mul(l_LightViewPosition, m_LightProjection[0]);
		
		float2 l_ProjectedLightCoords=float2(((l_LightViewPosition.x/l_LightViewPosition.w)/2.0f)+0.5f, ((-l_LightViewPosition.y/l_LightViewPosition.w)/2.0f)+0.5f);
		
		 // Determine if the projected coordinates are in the 0 to 1 range.
		if((saturate(l_ProjectedLightCoords.x)==l_ProjectedLightCoords.x) && (saturate(l_ProjectedLightCoords.y)==l_ProjectedLightCoords.y))
		{
			float l_DepthValue=T6Texture.Sample(S6Sampler, l_ProjectedLightCoords).r;
			float l_LightDepthValue=l_LightViewPosition.z/l_LightViewPosition.w;
			
			l_LightDepthValue = l_LightDepthValue - l_Bias;
			
			if(l_LightDepthValue < l_Depth)
			{
				l_LightIntensity = GetLightInt(0, l_WorldPos, l_WorldNormal);

				if(l_LightIntensity > 0.0)
				{
					color = float4(1.0f, 1.0f, 1.0f, 1.0f);
				}
			}
		}
	}
	
	return color;
}


