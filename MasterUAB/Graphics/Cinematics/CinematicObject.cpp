#include "Cinematics\CinematicObject.h"
#include "Utils\Utils.h"
#include "XML\XMLTreeNode.h"
#include "Engine\Engine.h"
#include "RenderableObjects\LayerManager.h"
#include "Log\Log.h"
#include "Math\MathUtils.h"
#include "Engine\Engine.h"
#include "PhysXManager.h"

CCinematicObject::CCinematicObject(CXMLTreeNode &TreeNode)
{
	m_CurrentKeyFrame=0;
	std::string l_Resource = TreeNode.GetPszProperty("resource");
	std::string l_Layer = TreeNode.GetPszProperty("layer");
	m_RenderableObject=CEngine::GetSingleton().GetLayerManager()->GetResource(l_Layer)->GetResource(l_Resource);
	//seleccionar el RO manager correcto. TreeNode.GetPszProperty("renderable_objects_manager");
	
	if(IsOk())
	{
		for (int i = 0; i < TreeNode.GetNumChildren(); ++i)
		{
			CXMLTreeNode l_Element = TreeNode(i);

			if (l_Element.GetName() == std::string("cinematic_object_key_frame"))
			{
				CCinematicObjectKeyFrame *l_COKeyFrame = new CCinematicObjectKeyFrame(l_Element);
				m_Duration=l_COKeyFrame->GetKeyFrameTime();

				m_CinematicObjectKeyFrames.push_back(l_COKeyFrame);
			}
		}
	}
}

bool CCinematicObject::IsOk()
{
	return m_RenderableObject!=NULL;
}

CCinematicObject::~CCinematicObject()
{
	for(size_t i=0;i<m_CinematicObjectKeyFrames.size();++i)
	{
		CHECKED_DELETE(m_CinematicObjectKeyFrames[i]);
	}
	m_CinematicObjectKeyFrames.clear();
}

void CCinematicObject::AddCinematicObjectKeyFrame(CCinematicObjectKeyFrame *CinematicObjectKeyFrame)
{
	m_CinematicObjectKeyFrames.push_back(CinematicObjectKeyFrame);
} 

Quatf CCinematicObject::Slerp(Quatf q0, Quatf q1,float t)
{
	Quatf l_Result(0.0f,0.0f,0.0f,1.0f);

	float cosOmega = q0.w*q1.w + q0.x*q1.x + q0.y*q1.y + q0.z*q1.z;

	// If negative dot, negate one of the input 
	// quaternions to take the shorter 4D "arc"

	if (cosOmega < 0.0f) 
	{ 
		q1.w = -q1.w; 
		q1.x = -q1.x; 
		q1.y = -q1.y; 
		q1.z = -q1.z; 
		cosOmega = -cosOmega; 
	}

	// Check if they are very close together to protect 
	// against divide-by-zero

	float k0, k1; 
	if (cosOmega > 0.9999f) 
	{
		// Very close - just use linear interpolation
		k0 = 1.0f-t; 
		k1 = t;
	} 
	else 
	{
		// Compute the sin of the angle using the // trig identity sin^2(omega) + cos^2(omega) = 1
		float sinOmega = sqrt(1.0f - cosOmega*cosOmega);
		
		// Compute the angle from its sin and cosine
		float omega = atan2(sinOmega, cosOmega);
		
		// Compute inverse of denominator, so we only have // to divide once
		float oneOverSinOmega = 1.0f / sinOmega;
		
		// Compute interpolation parameters
		k0 = sin((1.0f - t) * omega) * oneOverSinOmega;
		k1 = sin(t * omega) * oneOverSinOmega;
	}

	// Interpolate
	l_Result.w = q0.w*k0 + q1.w*k1; 
	l_Result.x = q0.x*k0 + q1.x*k1; 
	l_Result.y = q0.y*k0 + q1.y*k1; 
	l_Result.z = q0.z*k0 + q1.z*k1;

	return l_Result;
}

Vect3f CCinematicObject::ObjectToInertialQuaternion(Quatf q)
{
	Vect3f l_EulerAngles;
	float h=0.0f;
	float p=0.0f;
	float b=0.0f;

	// Extract sin(pitch)
	float sp = -2.0f * (q.y*q.z - q.w*q.x);

	// Check for Gimbal lock, giving slight tolerance for numerical imprecision
	if (fabs(sp) > 0.9999f) 
	{
		// Looking straight up or down
		p = 1.570796f * sp; // pi/2
		// Compute heading, slam bank to zero
		h = atan2(-q.x*q.z + q.w*q.y, 0.5f - q.y*q.y - q.z*q.z);
		b = 0.0f;
	} 
	else 
	{
		// Compute angles
		p = asin(sp);
		h = atan2(q.x*q.z + q.w*q.y, 0.5f - q.x*q.x - q.y*q.y);
		b = atan2(q.x*q.y + q.w*q.z, 0.5f - q.x*q.x - q.z*q.z);
	}

	l_EulerAngles(h,p,b);

	return l_EulerAngles;
}

void CCinematicObject::Update(float ElapsedTime)
{
	if (m_Playing)
	{
		m_CurrentTime = m_CurrentTime + ElapsedTime;

		CEngine::GetSingleton().GetLogManager()->Log(m_CurrentTime);

		for (size_t i = m_CurrentKeyFrame; i<m_CinematicObjectKeyFrames.size() - 1; ++i)
		{
			if (m_CinematicObjectKeyFrames[i]->GetKeyFrameTime()>m_CurrentTime)
			{
				m_CurrentKeyFrame = i;

			}
		}

		Vect3f l_NewPosition = v3fZERO;
		Vect3f l_Pos = m_CinematicObjectKeyFrames[m_CurrentKeyFrame]->GetPosition();
		l_NewPosition = (l_Pos.Lerp(m_CinematicObjectKeyFrames[m_CurrentKeyFrame + 1]->GetPosition(), m_CurrentTime));


		float h = m_CinematicObjectKeyFrames[m_CurrentKeyFrame]->GetYaw();
		float h2 = m_CinematicObjectKeyFrames[m_CurrentKeyFrame + 1]->GetYaw();

		float p = m_CinematicObjectKeyFrames[m_CurrentKeyFrame]->GetPitch();
		float p2 = m_CinematicObjectKeyFrames[m_CurrentKeyFrame + 1]->GetPitch();

		float b = m_CinematicObjectKeyFrames[m_CurrentKeyFrame]->GetRoll();
		float b2 = m_CinematicObjectKeyFrames[m_CurrentKeyFrame + 1]->GetRoll();

		Quatf l_q1(h, p, b);
		Quatf l_q2(h2, p2, b2);

		Quatf l_qInterpolated = Slerp(l_q1, l_q2, m_CurrentTime);

		CEngine &l_Engine = CEngine::GetSingleton();
		l_Engine.GetPhysXManager()->MoveKinematicActor(m_RenderableObject->GetName(), l_NewPosition, l_qInterpolated);
	}
}

void CCinematicObject::Stop()
{
	m_Playing=false;
	m_CurrentTime=0;
	m_CurrentKeyFrame=0;
}

void CCinematicObject::OnRestartCycle()
{
	m_CurrentTime=0;
	m_CurrentKeyFrame=0;
} 