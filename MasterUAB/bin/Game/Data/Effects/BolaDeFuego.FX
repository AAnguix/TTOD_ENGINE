#include "globals.fxh"
#include "samplers.fxh"
#include "functions.fxh"

static float3 m_Color1 = float3(m_RawDataValues[0], m_RawDataValues[1], m_RawDataValues[2]);
static float3 m_Color2 = float3(m_RawDataValues[3], m_RawDataValues[4], m_RawDataValues[5]);
static float m_AngleDenominator = m_RawDataValues[6];
static float m_FrequencyVar1 = m_RawDataValues[7];
static float m_FrequencyVar2 = m_RawDataValues[8];
static float m_Radius = m_RawDataValues[9];
static float m_StarGlow = m_RawDataValues[10];
static float m_SpecularFactor=m_RawDataValues[11];
static float m_Gloss=m_RawDataValues[12];
static float m_SsrFactor=m_RawDataValues[13];
static float m_Aspect = m_RawDataValues[14];
static float m_Scale = m_RawDataValues[15];

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
	float4 HPos : TEXCOORD2;
	float3 WorldPos : TEXCOORD3;
};

struct PixelOutputType
{
	float4 Target0 : SV_Target0; //Albedo (float3) + (float) SpecularFactor
	float4 Target1 : SV_Target1; //AmbientLight (float3) + (float) SpecularPow
	float4 Target2 : SV_Target2; //Normal (float3) + (float) Not used
	float4 Target3 : SV_Target3; //Depth (float4)
};

float GLSL_modF3(float3 x, float y)
{
	float3 lDivision = floor(x/y);
  return x - (y * lDivision);
}

float snoise(float3 uv, float res)
{
	const float3 s = float3(1e0, 1e2, 1e4);
	
	uv = mul(uv,res);
	
	float3 uv0 = floor(GLSL_modF3(uv, res))*s;
	float3 uv1 = floor(GLSL_modF3(uv+float3(1.0,1.0,1.0), res))*s;
	
	float3 f = frac(uv); f = f*f*(3.0-2.0*f);
	float4 v = float4(uv0.x+uv0.y+uv0.z, uv1.x+uv0.y+uv0.z,
		      	  uv0.x+uv1.y+uv0.z, uv1.x+uv1.y+uv0.z);
	
	float4 r = frac(sin(v*1e-3)*1e5);
	float r0 = lerp(lerp(r.x, r.y, f.x), lerp(r.z, r.w, f.x), f.y);
	
	r = frac(sin((v + uv1.z - uv0.z)*1e-3)*1e5);
	float r1 = lerp(lerp(r.x, r.y, f.x), lerp(r.z, r.w, f.x), f.y);
	
	return lerp(r0, r1, f.z)*2.-1.;
}

PS_INPUT VS( VS_INPUT IN )
{
	PS_INPUT l_Output = (PS_INPUT)0;
	float l_Factor = 0.5;
	float l_InverseSpeed = 4.0;
	//float l_NewScale=cos(m_Times.y/l_InverseSpeed)*l_Factor;
	float l_NewScale=m_Scale;
	float4 l_Pos=mul(float4(IN.Pos*l_NewScale,1.0), m_World );
	l_Output.WorldPos = l_Pos.xyz;
	float4 l_HPos=mul(l_Pos, m_View);
	l_HPos=mul(l_HPos, m_Projection);
	l_Output.Pos=l_HPos;
	l_Output.HPos = l_HPos; 
	l_Output.Normal = normalize(mul(normalize(IN.Normal).xyz, (float3x3)m_World));
	l_Output.UV = IN.UV;
	
	return l_Output;
}


PixelOutputType PS( PS_INPUT IN) : SV_Target
{
	PixelOutputType l_Output;

	float4 Fire_result = float4(0.0,0.0,0.0,0.0);
	
	float brightness = m_FrequencyVar1 * 0.25 + m_FrequencyVar2 * 0.25;
	float radius = 0.24 + brightness * 0.2;
	float invRadius  = 1.0/radius;
	
	float time = m_Times.y * 0.1;
	
	//float aspect	= iResolution.x/iResolution.y;
	//float2 uv	= fragCoord.xy / iResolution.xy;
	float2 uv = IN.UV;
	float2 p = uv - 0.5;
	p.x = mul(p.x,m_Aspect);
	
	float fade	= pow( length( 2.0 * p ), 0.5 );
	float fVal1	= 1.0 - fade;
	float fVal2	= 1.0 - fade;
	
	float angle = atan2(p.y,p.x)/m_AngleDenominator;
	float dist = length(p);
	float3 coord = float3( angle, dist, time * 0.1 );
	
	float newTime1	= abs( snoise( coord + float3( 0.0, -time * ( 0.35 + brightness * 0.001 ), time * 0.015 ), 15.0 ) );
	float newTime2	= abs( snoise( coord + float3( 0.0, -time * ( 0.15 + brightness * 0.001 ), time * 0.015 ), 45.0 ) );	
	for( int i=1; i<=7; i++ )
	{
		float power = pow( 2.0, float(i + 1) );
		fVal1 += ( 0.5 / power ) * snoise( coord + float3( 0.0, -time, time * 0.2 ), ( power * ( 10.0 ) * ( newTime1 + 1.0 ) ) );
		fVal2 += ( 0.5 / power ) * snoise( coord + float3( 0.0, -time, time * 0.2 ), ( power * ( 25.0 ) * ( newTime2 + 1.0 ) ) );
	}
	
	float corona = pow( fVal1 * max( 1.1 - fade, 0.0 ), 2.0 ) * 50.0;
	corona += pow( fVal2 * max( 1.1 - fade, 0.0 ), 2.0 ) * 50.0;
	corona *= 1.2 - newTime1;
	
	float3 sphereNormal = float3( 0.0, 0.0, 1.0 );
	float3 dir = float3( 0.0,0.0,0.0 );
	float3 center = float3( 0.5, 0.5, 1.0 );
	float3 starSphere = float3( 0.0,0.0,0.0 );
	
	float2 sp = -1.0 + 2.0 * uv;
	sp.x *= m_Aspect;
	sp *= ( 2.0 - brightness );
  	float r = dot(sp,sp);
	float f = (1.0-sqrt(abs(1.0-r)))/(r) + brightness * 0.5;
	if( dist < radius )
	{
		corona *= pow( abs(dist) * invRadius, 24.0 );
  		float2 newUv;
 		newUv.x = sp.x*f;
  		newUv.y = sp.y*f;
		newUv += float2( time, 0.0 );
		
		float3 texSample = T0Texture.Sample(S0Sampler, newUv).rgb;
		float l_UVDisplacement = ( texSample.g * brightness * 4.5 + time );
		float2 l_StarUV = newUv + float2( l_UVDisplacement, 0.0 );
		starSphere 	= T0Texture.Sample(S0Sampler, l_StarUV).rgb;
	}
	
	//float starGlow	= min( max( 1.0 - dist * ( 1.0 - brightness ), 0.0 ), 1.0 );
	float starGlow	= m_StarGlow;
	//Fire_result.rgb	= float3( r );
	Fire_result.rgb	= float3( f * ( 0.75 + brightness * 0.3 ) * m_Color1 ) 
	+ starSphere + corona * m_Color1 + starGlow * m_Color2;
	
	Fire_result.a		= 1.0;
	
	float l_Spec = 0.0;
	
	float l_Depth = IN.HPos.z / IN.HPos.w;
	float4 l_Diffuse = T0Texture.Sample(S0Sampler, IN.UV);

	//l_Output.Target0.xyz = l_Diffuse.xyz;
	l_Output.Target0.xyz = Fire_result.xyz;
	l_Output.Target1.xyz = float3(m_LightAmbient.xyz*l_Output.Target0.xyz);
	
	l_Output.Target0.w = m_SpecularFactor * (1-l_Spec);
	l_Output.Target1.w = 1/m_Gloss;
	l_Output.Target2.w = m_SsrFactor;
	
	l_Output.Target2 = float4(Normal2Texture(IN.Normal.xyz), 1.0f);
	l_Output.Target3=float4(l_Depth, l_Depth, l_Depth, 1.0);
	return l_Output;
	
}
