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

float4 PS(PS_INPUT IN) : SV_Target
{
	float4 color = float4(0,0,0,1);
	
	if(m_UseShadowMapArray[0])
	{	
		float l_Depth = T0Texture.Sample(S0Sampler, IN.UV).r;
	
		float3 l_WorldPos=GetPositionFromZDepthView(l_Depth, IN.UV,m_InverseView, m_InverseProjection);
		
		//Pixel position in light viewProjection coordinates
		float4 l_LightViewPosition=mul(float4(l_WorldPos, 1), m_LightView[0]);
		l_LightViewPosition=mul(l_LightViewPosition, m_LightProjection[0]);
		
		float2 l_ProjectedLightCoords=float2(((l_LightViewPosition.x/l_LightViewPosition.w)/2.0f)+0.5f, ((-l_LightViewPosition.y/l_LightViewPosition.w)/2.0f)+0.5f);
		
		 // Determine if the projected coordinates are in the 0 to 1 range.
		if((saturate(l_ProjectedLightCoords.x)==l_ProjectedLightCoords.x) && (saturate(l_ProjectedLightCoords.y)==l_ProjectedLightCoords.y))
		{
			float l_ShadowMap=T6Texture.Sample(S6Sampler, l_ProjectedLightCoords).r;
			float l_LightDepthValue=l_LightViewPosition.z/l_LightViewPosition.w;
			
			//color = float4(l_ShadowMap, l_ShadowMap, l_ShadowMap, l_ShadowMap);
			if(l_LightDepthValue < l_ShadowMap) //<
			{
				color = float4(1, 1, 1, 1);
			}
		}
	}
	
	return color;
}


