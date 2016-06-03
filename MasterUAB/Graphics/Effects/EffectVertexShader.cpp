#include "Effects\EffectVertexShader.h"
#include "Render\RenderManager.h"
#include "Engine.h"
#include "Vertex\VertexTypes.h"
#include "Log\Log.h"
#include "RenderableObjects\RenderableObjectTechniqueManager.h"
#include "XML\XMLTreeNode.h"

void CEffectVertexShader::Destroy()
{
	if (m_VertexLayout)
	{
		m_VertexLayout->Release();
		m_VertexLayout = 0;
	}

	if (m_VertexShader)
	{
		m_VertexShader->Release();
		m_VertexShader = 0;
	}
}

CEffectVertexShader::~CEffectVertexShader()
{
	Destroy();
}

CEffectVertexShader::CEffectVertexShader(const CXMLTreeNode &TreeNode)
: CEffectShader(TreeNode)
, m_VertexShader(NULL)
, m_VertexLayout(NULL)
{
	m_ShaderModel = TreeNode.GetPszProperty("shader_model","vs_4_0");
	m_EntryPoint = TreeNode.GetPszProperty("entry_point","VS");
	m_VertexType = TreeNode.GetPszProperty("vertex_type", "");
}

bool CEffectVertexShader::Load()
{  
	assert(m_VertexType != "");
	
	CreateShaderMacro();

	ID3DBlob *l_VSBlob=NULL;  
	bool l_Loaded=LoadShader(m_Filename, m_EntryPoint, m_ShaderModel, &l_VSBlob);  
	if(!l_Loaded) 
	{
		CEngine::GetSingleton().GetLogManager()->Log("Unable to load vertex shader "+m_Filename);
		assert(false);
		return false;  
	}
	CRenderManager *l_RenderManager=CEngine::GetSingleton().GetRenderManager();
	ID3D11Device *l_Device=l_RenderManager->GetContextManager()->GetDevice();
	HRESULT l_HR=l_Device->CreateVertexShader(l_VSBlob->GetBufferPointer(), l_VSBlob->GetBufferSize(), NULL, &m_VertexShader);  
	
	if( FAILED(l_HR) )  
	{   
		l_VSBlob->Release(); 
		assert(false);
		return false;  
	}  
 
	l_Loaded = false;

	if(m_VertexType=="MV_POSITION_NORMAL_TEXTURE_VERTEX")  
		l_Loaded=MV_POSITION_NORMAL_TEXTURE_VERTEX::CreateInputLayout(l_RenderManager, l_VSBlob, &m_VertexLayout);  

	else if(m_VertexType=="MV_POSITION_WEIGHT_INDICES_NORMAL_TEXTURE_VERTEX")  
		l_Loaded=MV_POSITION_WEIGHT_INDICES_NORMAL_TEXTURE_VERTEX::CreateInputLayout(l_RenderManager, l_VSBlob, &m_VertexLayout);  

	else if(m_VertexType=="MV_POSITION4_COLOR_TEXTURE_VERTEX")  
		l_Loaded=MV_POSITION4_COLOR_TEXTURE_VERTEX::CreateInputLayout(l_RenderManager, l_VSBlob, &m_VertexLayout);  
	
	else if(m_VertexType=="MV_POSITION_COLOR_VERTEX")  
		l_Loaded=MV_POSITION_COLOR_VERTEX::CreateInputLayout(l_RenderManager, l_VSBlob, &m_VertexLayout);  

	else if(m_VertexType=="MV_POSITION_TEXTURE_VERTEX")  
		l_Loaded=MV_POSITION_TEXTURE_VERTEX::CreateInputLayout(l_RenderManager, l_VSBlob, &m_VertexLayout);  

	else if(m_VertexType=="MV_POSITION_COLOR_TEXTURE_VERTEX")  
		l_Loaded=MV_POSITION_COLOR_TEXTURE_VERTEX::CreateInputLayout(l_RenderManager, l_VSBlob, &m_VertexLayout);  

	else if(m_VertexType=="MV_POSITION_NORMAL_TEXTURE_TEXTURE2_VERTEX")  
		l_Loaded=MV_POSITION_NORMAL_TEXTURE_TEXTURE2_VERTEX::CreateInputLayout(l_RenderManager, l_VSBlob, &m_VertexLayout);  
	
	else if(m_VertexType=="MV_POSITION_NORMAL_TEXTURE_BINORMAL_TANGENT_VERTEX")  
		l_Loaded=MV_POSITION_NORMAL_TEXTURE_BINORMAL_TANGENT_VERTEX::CreateInputLayout(l_RenderManager, l_VSBlob, &m_VertexLayout);  
	
	else if(m_VertexType=="MV_POSITION_NORMAL_TEXTURE_TEXTURE2_BINORMAL_TANGENT_VERTEX")  
		l_Loaded=MV_POSITION_NORMAL_TEXTURE_TEXTURE2_BINORMAL_TANGENT_VERTEX::CreateInputLayout(l_RenderManager, l_VSBlob, &m_VertexLayout);  
	
	else if (m_VertexType == "MV_PARTICLE_VERTEX")
		l_Loaded = MV_PARTICLE_VERTEX::CreateInputLayout(l_RenderManager, l_VSBlob, &m_VertexLayout);
	else
	{
		CEngine::GetSingleton().GetLogManager()->Log("Vertex type " + m_VertexType + " not recognized on CEffectVertexShader::Load");
		assert(false);
	}
	//else if (m_VertexType == "MV_POSITION_WEIGHT_INDICES_NORMAL_BINORMAL_TANGENT_TEXTURE_VERTEX")
		//l_Loaded = MV_POSITION_WEIGHT_INDICES_NORMAL_BINORMAL_TANGENT_TEXTURE_VERTEX::CreateInputLayout(l_RenderManager, l_VSBlob, &m_VertexLayout);
	
	l_VSBlob->Release();  
	if (!l_Loaded)
	{
		CEngine::GetSingleton().GetLogManager()->Log("Can't create input layout for vertex type" + m_VertexType + ". " + m_Filename);
		assert(false);
		return false;
	}
	return CreateConstantBuffer(); 
} 

void CEffectVertexShader::SetConstantBuffer(unsigned int IdBuffer, void *ConstantBuffer)
{  
	ID3D11DeviceContext *l_DeviceContext=CEngine::GetSingleton().GetRenderManager()->GetContextManager()->GetDeviceContext(); 
	ID3D11Buffer *l_ConstantBuffer=GetConstantBuffer(IdBuffer);    
	
	if(l_ConstantBuffer!=NULL)  
	{   
		l_DeviceContext->UpdateSubresource(l_ConstantBuffer, 0, NULL, ConstantBuffer, 0, 0);   
		l_DeviceContext->VSSetConstantBuffers(IdBuffer, 1, &l_ConstantBuffer);  
	}
}