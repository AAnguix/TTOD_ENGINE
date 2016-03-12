#include "Particles\ParticleSystemInstance.h"
#include "Particles\ParticleSystemType.h"
#include "XML\XMLTreeNode.h"

CParticleSystemInstance::CParticleSystemInstance(CXMLTreeNode &TreeNode) : m_ActiveParticles(0), m_RandomEngine(rnd), m_UnitDistribution(0.0f,1.0f)
{
	m_EmissionBoxHalfSize = TreeNode.GetVect3fProperty("emission_box_size", Vect3f(1, 1, 1)*0.5f);
	m_EmissionVolume = m_EmissionBoxHalfSize.x * m_EmissionBoxHalfSize.y * m_EmissionBoxHalfSize.z * 8;
	m_EmissionScaler = m_Type->m_EmitAbsolute ? 1 : 1.0f / m_EmissionVolume;
}

void CParticleSystemInstance::Render(CRenderManager *RenderManager)
{

}

void CParticleSystemInstance::Update(float ElapsedTime)
{
	//generar particulas

	//eliminar particulas que transcurra todo su tiempo de vida

	//actualizar estado partículas vivas

	//ordenar particulas para renderizarlas correctamente

	//traducir info a formato que entienda el shader
}
