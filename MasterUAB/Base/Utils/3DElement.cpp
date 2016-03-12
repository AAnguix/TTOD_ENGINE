#include "3DElement.h"

/*
Vect3f  m_Position, m_PrevPos;  
	float  m_Yaw, m_Pitch, m_Roll;  
	bool  m_TranslationUpdate, m_RotationUpdate, m_ScaleUpdate;  
	Mat44f  m_TransformMatrix, m_RotationMatrix, m_TranslationMatrix, m_ScaleMatrix;  
	Vect3f  m_Scale; 

	bool m_Visible;
	*/
C3DElement::C3DElement(): 
	m_Position(v3fZERO),
	m_PrevPos(v3fZERO),
	m_Yaw(0.0f),
	m_Pitch(0.0f),
	m_Roll(0.0f),
	m_Scale(v3fZERO),
	m_Visible(true)
{
	m_Position=v3fZERO;
	m_Yaw=0.0f;
	m_Pitch=0.0f;
	m_Roll=0.0f;
	m_Visible=true;
}
C3DElement::C3DElement(const Vect3f &Position)
{
	m_Position=Position;
	m_Visible=true;
}
C3DElement::C3DElement(const Vect3f &Position, float Yaw, float Pitch, float Roll)
{
	m_Position=Position;
	m_Yaw=Yaw;
	m_Pitch=Pitch;
	m_Roll=Roll;
	m_Visible=true;
}
  
C3DElement::C3DElement(float Yaw, float Pitch, float Roll)
{
	m_Yaw=Yaw;
	m_Pitch=Pitch;
	m_Roll=Roll;
	m_Visible=true;
}

C3DElement::C3DElement(const CXMLTreeNode &XMLTreeNode)
{
	m_Position=XMLTreeNode.GetVect3fProperty("pos", v3fZERO);
	
	m_Yaw=XMLTreeNode.GetFloatProperty("yaw",0.0f);
	m_Pitch=XMLTreeNode.GetFloatProperty("pitch",0.0f);
	m_Roll=XMLTreeNode.GetFloatProperty("roll",0.0f);
	
	m_Scale=XMLTreeNode.GetVect3fProperty("scale",0.0f);

	m_Visible=true;
} 

void C3DElement::SetPosition(const Vect3f &Position)
{
	m_Position=Position;
}  

float C3DElement::GetYaw() const
{
	return m_Yaw;
}

float C3DElement::GetPitch() const
{
	return m_Pitch;
}  

float C3DElement::GetRoll() const
{
	return m_Roll;
}

Vect3f C3DElement::GetUp() const
{
	/*return Vect3f(-sin(m_Roll) * cos(m_Roll) + cos(m_Roll) * sin(m_Yaw) * sin(m_Roll),
	cos(m_Roll) * cos(m_Roll) + sin(m_Roll) * sin(m_Yaw) * sin(m_Roll),
	cos(m_Yaw) * sin(m_Roll) ); */

	return Vect3f(m_TransformMatrix.m01,m_TransformMatrix.m11,m_TransformMatrix.m21);
}

Vect3f C3DElement::GetRight() const
{
	/*return Vect3f(cos(m_Yaw)*cos(m_Roll),
	sin(m_Roll)*cos(m_Yaw),
	-sin(m_Yaw));*/
	
	return Vect3f(m_TransformMatrix.m00,m_TransformMatrix.m10,m_TransformMatrix.m20);
}

Vect3f C3DElement::GetForward() const
{
	/*return Vect3f( sin(m_Roll) * sin(m_Roll) + cos(m_Roll) * sin(m_Yaw) * cos(m_Roll),
	-cos(m_Roll) * sin(m_Roll) + sin(m_Roll) * sin(m_Yaw) * cos(m_Roll),
	cos(m_Yaw) * cos(m_Roll) ); */

	return Vect3f(m_TransformMatrix.m02,m_TransformMatrix.m12,m_TransformMatrix.m22);
}

Vect3f C3DElement::GetTranslation()  const
{
	return Vect3f(m_TransformMatrix.m03,m_TransformMatrix.m13,m_TransformMatrix.m23);
}


/*
Vect3f C3DElement::GetForward() const
{
	return Vect3f( (cos(m_Pitch)*sin(m_Yaw)), (-sin(m_Pitch)), (cos(m_Yaw)*cos(m_Pitch)) ); //ok //Roll no afecta
}

Vect3f C3DElement::GetUp() const
{
	return Vect3f(0.0f,1.0f,0.0f);
	//return Vect3f(-sin(m_Pitch),cos(m_Roll)*cos(m_Pitch),sin(m_Pitch) * ); //Yaw no afecta
}

Vect3f C3DElement::GetRight() const
{
	return Vect3f(cos(m_Yaw)*cos(m_Roll),cos(m_Roll),-sin(m_Yaw)); //Pitch no afecta
}
*/

void C3DElement::SetYaw(float Yaw)
{
	m_Yaw=Yaw;
}  

void C3DElement::SetPitch(float Pitch)
{
	m_Pitch=Pitch;
}  

void C3DElement::SetRoll(float Roll)
{
	m_Roll=Roll;
}  

void C3DElement::SetYawPitchRoll(float Yaw, float Pitch, float Roll)
{
	m_Yaw=Yaw;
	m_Pitch=Pitch;
	m_Roll=Roll;
} 

void C3DElement::SetScale(const Vect3f &Scale)
{
	m_Scale=Scale;
} 

/*
void C3DElement::Render(crender *RenderManager)
{

}*/

const Mat44f & C3DElement::GetTransform()
{
	m_TransformMatrix.SetIdentity();
	m_TransformMatrix.SetFromPosAndAnglesYXZ(m_Position, m_Yaw, m_Pitch, m_Roll);
	return m_TransformMatrix;
} 

bool C3DElement::GetVisible() const
{
	return m_Visible;
}

void C3DElement::SetVisible(bool Visible)
{
	m_Visible=Visible;
}