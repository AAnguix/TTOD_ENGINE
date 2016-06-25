float3 Normal2Texture(float3 Normal)
{
	return Normal*0.5+0.5;
}

float3 Texture2Normal(float3 Color)
{
	return (Color-0.5)*2;
}

float4 Red()
{
	return float4(1.0,0.0,0.0,1.0);
}


float4 Green()
{
	return float4(0.0,1.0,0.0,1.0);
}


float4 Blue()
{
	return float4(0.0,0.0,1.0,1.0);
}

float3 GetPositionFromZDepthViewInViewCoordinates(float ZDepthView, float2 UV, float4x4 InverseProjection)
{
	// Get the depth value for this pixel
	// Get x/w and y/w from the viewport position
	float x = UV.x * 2 - 1;
	float y = (1 - UV.y) * 2 - 1;
	float4 l_ProjectedPos = float4(x, y, ZDepthView, 1.0);
	// Transform by the inverse projection matrix
	float4 l_PositionVS = mul(l_ProjectedPos, InverseProjection);
	// Divide by w to get the view-space position
	return l_PositionVS.xyz / l_PositionVS.w;
}

float3 GetPositionFromZDepthView(float ZDepthView, float2 UV, float4x4 InverseView, float4x4 InverseProjection)
{
	float3 l_PositionView=GetPositionFromZDepthViewInViewCoordinates(ZDepthView, UV, InverseProjection);
	return mul(float4(l_PositionView,1.0), InverseView).xyz;
}

float3 GetRadiosityNormalMap(float3 Nn, float2 UV, Texture2D LightmapXTexture, SamplerState
LightmapXSampler, Texture2D LightmapYTexture, SamplerState LightmapYSampler, Texture2D
LightmapZTexture, SamplerState LightmapZSampler)
{
	float3 l_LightmapX=LightmapXTexture.Sample(LightmapXSampler, UV)*2;
	float3 l_LightmapY=LightmapYTexture.Sample(LightmapYSampler, UV)*2;
	float3 l_LightmapZ=LightmapZTexture.Sample(LightmapZSampler, UV)*2;
	float3 l_BumpBasisX=normalize(float3(0.816496580927726, 0.5773502691896258, 0 ));
	float3 l_BumpBasisY=normalize(float3(-0.408248290463863,  0.5773502691896258,0.7071067811865475 ));
	float3 l_BumpBasisZ=normalize(float3(-0.408248290463863, 0.5773502691896258, -0.7071067811865475));
	float3 l_RNMLighting=saturate(dot(Nn, l_BumpBasisX))*l_LightmapX+
	saturate(dot(Nn, l_BumpBasisY))*l_LightmapY+
	saturate(dot(Nn, l_BumpBasisZ))*l_LightmapZ;
	return l_RNMLighting;
}