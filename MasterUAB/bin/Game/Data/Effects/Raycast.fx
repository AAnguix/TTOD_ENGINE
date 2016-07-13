#include "globals.fxh"
#include "samplers.fxh"

static float3 m_StartPosition=float3(m_RawDataValues[0], m_RawDataValues[1], m_RawDataValues[2]);
static float3 m_EndPosition=float3(m_RawDataValues[3], m_RawDataValues[4], m_RawDataValues[5]);
// static float2 m_Intensity = float(m_RawDataValues[6])

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
	PS_INPUT l_Output = (PS_INPUT)0;
	
	float4x4 m_ModelView = mul(m_World,m_View);

	m_ModelView[0][0] = 1.0; 
	m_ModelView[0][2] = 0.0; 
	
	m_ModelView[2][0] = 0.0; 
	m_ModelView[2][2] = 1.0; 
	
	l_Output.Pos = mul(float4(IN.Pos.xyz,1.0), m_ModelView);
	l_Output.Pos = mul(l_Output.Pos, m_Projection );
	
	l_Output.Normal = normalize(mul(normalize(IN.Normal).xyz, (float3x3)m_World));
	l_Output.UV = IN.UV;
	l_Output.WorldTangent = mul(IN.Tangent.xyz,(float3x3)m_World);
	l_Output.WorldBinormal = mul(cross(IN.Tangent.xyz,IN.Normal),(float3x3)m_World);
	
	return l_Output;
}


float PointInsideCilinder(float3 pt1, float3 pt2, float lengthsq, float radius_sq, float3 testpt)
{
	float dx, dy, dz;	// vector d  from line segment point 1 to point 2
	float pdx, pdy, pdz;	// vector pd from point 1 to test point
	float dot, dsq;

	dx = pt2.x - pt1.x;	// translate so pt1 is origin.  Make vector from
	dy = pt2.y - pt1.y;     // pt1 to pt2.  Need for this is easily eliminated
	dz = pt2.z - pt1.z;

	pdx = testpt.x - pt1.x;		// vector from pt1 to test point.
	pdy = testpt.y - pt1.y;
	pdz = testpt.z - pt1.z;

	// Dot the d and pd vectors to see if point lies behind the 
	// cylinder cap at pt1.x, pt1.y, pt1.z

	dot = pdx * dx + pdy * dy + pdz * dz;

	// If dot is less than zero the point is behind the pt1 cap.
	// If greater than the cylinder axis line segment length squared
	// then the point is outside the other end cap at pt2.

	if( dot < 0.0f || dot > lengthsq )
	{
		return( -1.0f );
	}
	else 
	{
		dsq = (pdx*pdx + pdy*pdy + pdz*pdz) - dot*dot/lengthsq;

		if( dsq > radius_sq )
		{
			return( -1.0f );
		}
		else
		{
			return( dsq );		
		}
	}
}

float4 PS( PS_INPUT IN) : SV_Target
{
	// float4 l_Result = T0Texture.Sample(S0Sampler, IN.UV);
	// return l_Result;
	// float l_Lenght = length(m_EndPosition - m_StartPosition);
	
	//float3 pt1, float3 pt2, float lengthsq, float radius_sq, float3 testpt
	
	float l_Lenghtsq = sqrt(length(m_EndPosition-m_StartPosition));
	if (PointInsideCilinder(m_StartPosition,m_EndPosition,l_Lenghtsq,0.3,IN.WorldPos) == -1.0f)
	{
		return float4(0.0,1.0,0.0,1.0);
	}

	return float4(1.0,0.0,0.0,1.0);
}



