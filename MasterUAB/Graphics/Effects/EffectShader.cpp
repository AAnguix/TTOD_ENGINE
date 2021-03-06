#include "Effects\EffectShader.h"
#include "Render\RenderManager.h"
#include "Engine\Engine.h"
#include <d3dcompiler.h>
#include "XML\XMLTreeNode.h"
#include "Render\ContextManager.h"

#include "Log\Log.h"
#include "Utils\FileUtils.h"
#include "Effects\EffectVertexShader.h"
#include "Effects\EffectPixelShader.h"
#include "Effects\EffectGeometryShader.h"

CEffectShader::CEffectShader(const CXMLTreeNode &TreeNode)
:CNamed(TreeNode)
,m_PreprocessorMacros(NULL)
,m_ShaderMacros(nullptr)
,m_Preprocessor(TreeNode.GetPszProperty("macro", ""))
,m_Filename(TreeNode.GetPszProperty("file", ""))
{
}

CEffectShader::~CEffectShader()
{
	Destroy();
}

void CEffectShader::Destroy()
{
	if (m_ShaderMacros != nullptr)
	{
		delete m_ShaderMacros;
		m_ShaderMacros = nullptr;
	}
	
	for(size_t i=0;i<m_ConstantBuffers.size();++i)
	{
		if (m_ConstantBuffers[i])
		{
			m_ConstantBuffers[i]->Release();
			m_ConstantBuffers[i] = nullptr;
		}
	}
}

/*Del string separados por ; crea vector strings*/
/*Si encuentra un elemento, HAS_NORMAL,etc
a�ades la macro mas un 1.
Si tienes dos elementos, es porque cumplen el formato
DEBUG_COLOR=float4(1,1,0,1), a�adirias
DEBUG_COLOR y despues float4(1,1,0,1)*/
/*Los guarda en vector strings para que cuando el shader quiera usarlos
esten disponibles*/

void CEffectShader::SplitString(const std::string String,char Separator,std::vector<std::string> &Array)
{
	size_t l_LastSeparatorIndex=0;
	size_t l_Size = String.size();
	for (size_t i=0; i<l_Size; i++) 
	{
		if (String[i] == Separator ) 
		{
			Array.push_back(String.substr(l_LastSeparatorIndex,i-l_LastSeparatorIndex));
			l_LastSeparatorIndex=i;
		}
		else if (i==l_Size-1)
		{
			if(l_LastSeparatorIndex!=0)
				Array.push_back(String.substr(l_LastSeparatorIndex+1,i-l_LastSeparatorIndex+1));
			else Array.push_back(String.substr(l_LastSeparatorIndex,i+1));
		}
	}
	int a=0;
}


void CEffectShader::CreateShaderMacro()
{
	m_PreprocessorMacros.clear();  
	if(m_Preprocessor.empty())  
	{   
		m_ShaderMacros=NULL;   
		return;  
	}  
	
	std::vector<std::string> l_PreprocessorItems;  
	SplitString(m_Preprocessor, ';', l_PreprocessorItems);  
	m_ShaderMacros=new D3D10_SHADER_MACRO[l_PreprocessorItems.size()+1];  
	
	for(size_t i=0;i<l_PreprocessorItems.size();++i)  
	{   
		 std::vector<std::string> l_PreprocessorItem;   
		 SplitString(l_PreprocessorItems[i], '=', l_PreprocessorItem);
		 if(l_PreprocessorItem.size()==1)   
		 {    
			 m_PreprocessorMacros.push_back(l_PreprocessorItems[i]);    
			 m_PreprocessorMacros.push_back("1");   
		 }   
		 else if(l_PreprocessorItem.size()==2)   
		 {    
			m_PreprocessorMacros.push_back(l_PreprocessorItem[0]);    
			m_PreprocessorMacros.push_back(l_PreprocessorItem[1]);   
		 }   
		 else    
		 {   
			CEngine::GetSingleton().GetLogManager()->Log("Error creating shader macro '%s', with wrong size on parameters"+l_PreprocessorItems[i]);  
			if (m_ShaderMacros != nullptr)
			{
				delete m_ShaderMacros;
				m_ShaderMacros = nullptr;
			}
			return;   
		 }  
	}  
	for (size_t i = 0; i<l_PreprocessorItems.size(); ++i)
	{
		m_ShaderMacros[i].Name = m_PreprocessorMacros[i * 2].c_str();
		m_ShaderMacros[i].Definition = m_PreprocessorMacros[(i * 2) + 1].c_str();
	}

	m_ShaderMacros[l_PreprocessorItems.size()].Name=NULL;  
	m_ShaderMacros[l_PreprocessorItems.size()].Definition=NULL;
}

bool CEffectShader::Reload()
{
	Destroy();
	size_t l_EffectsStateCode = CEngine::GetSingleton().GetEffectManager()->GetEffectsStateCode();
	return Load(l_EffectsStateCode);
}


bool CEffectShader::LoadShaderExtended(const std::string &ShaderName, const std::string &Filename, const std::string &EntryPoint, const std::string &ShaderModel, ID3DBlob **BlobOut, size_t EffectsStateCode)
{
	//bool l_ShaderCompiled = CFileUtils::CompiledShaderExists(Filename, l_CompiledShaderFilename, EntryPoint);
	bool l_ShaderCompiled = CFileUtils::CompiledShaderExists(ShaderName);
	
	std::string l_CompiledName = "Data\\Effects\\compiled\\" + ShaderName;

	if ((l_ShaderCompiled && (CFileUtils::ShaderFileModified(Filename, l_CompiledName)) || EffectsStateCode==2) || (!l_ShaderCompiled))
	{
		HRESULT l_Hr = S_OK;
		ID3DBlob* l_ErrorBlob = nullptr;

		DWORD l_ShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;

		#if defined( DEBUG ) || defined( _DEBUG )  
			l_ShaderFlags |= D3DCOMPILE_DEBUG | D3DCOMPILE_PREFER_FLOW_CONTROL | D3DCOMPILE_SKIP_OPTIMIZATION;
		#endif  

		std::wstring ws;
		ws.assign(Filename.begin(), Filename.end());
		l_Hr = D3DCompileFromFile(ws.c_str(), m_ShaderMacros, D3D_COMPILE_STANDARD_FILE_INCLUDE, EntryPoint.c_str(), ShaderModel.c_str(), l_ShaderFlags, 0, BlobOut, &l_ErrorBlob);
		
		if (FAILED(l_Hr))
		{ 
			std::wstring l_ErrorsFile;
			std::string l_Erros = "Data/ShaderErrors";
			l_ErrorsFile.assign(l_Erros.begin(), l_Erros.end());
			D3DWriteBlobToFile(l_ErrorBlob, l_ErrorsFile.c_str(), true);
			//CEngine::GetSingleton().GetLogManager()->Log(static_cast<char>(l_ErrorBlob->GetBufferPointer()));
			assert(false);
			return false;
		}

		std::wstring l_WCompiledName;
		l_WCompiledName.assign(l_CompiledName.begin(), l_CompiledName.end());
		D3DWriteBlobToFile(*BlobOut, l_WCompiledName.c_str(), true);

		if (FAILED(l_Hr))
		{
			if (l_ErrorBlob != NULL)
				OutputDebugStringA((char*)l_ErrorBlob->GetBufferPointer());
			if (l_ErrorBlob)
				l_ErrorBlob->Release();
			return false;
		}

		if (l_ErrorBlob)
			l_ErrorBlob->Release();
	}
	else
	{
		std::wstring l_Str;
		l_Str.assign(l_CompiledName.begin(), l_CompiledName.end());
		HRESULT l_H = D3DReadFileToBlob(l_Str.c_str(), BlobOut);
	}
	return true;
}

bool CEffectShader::LoadShader(const std::string &Filename, const std::string &EntryPoint, const std::string &ShaderModel, ID3DBlob **BlobOut)
{
	HRESULT hr = S_OK;
	ID3DBlob* pErrorBlob = nullptr;

	UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;

	#if defined( DEBUG ) || defined( _DEBUG )  
		dwShaderFlags |= D3DCOMPILE_DEBUG;
	#endif  

	std::wstring ws;
	ws.assign(Filename.begin(), Filename.end());

	hr = D3DCompileFromFile(ws.c_str(), m_ShaderMacros, D3D_COMPILE_STANDARD_FILE_INCLUDE, EntryPoint.c_str(), ShaderModel.c_str(), dwShaderFlags, 0, BlobOut, &pErrorBlob);

	if (FAILED(hr))
	{
		if (pErrorBlob != NULL)
			OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
		if (pErrorBlob)
			pErrorBlob->Release();
		return false;
	}

	if (pErrorBlob)
		pErrorBlob->Release();

	return true;
}

/*Copy from RAM to VRAM*/
bool CEffectShader::CreateConstantBuffer(int IdBuffer, unsigned int BufferSize)
{
	ID3D11Buffer *l_ConstantBuffer;  
	CRenderManager *l_RenderManager=CEngine::GetSingleton().GetRenderManager();  
	ID3D11Device *l_Device=l_RenderManager->GetContextManager()->GetDevice(); 

	D3D11_BUFFER_DESC l_BufferDescription;  
	ZeroMemory(&l_BufferDescription, sizeof(l_BufferDescription));  
	l_BufferDescription.Usage=D3D11_USAGE_DEFAULT;  
	l_BufferDescription.ByteWidth=BufferSize;  
 
	//if((BufferSize%16)!=0)   
		//Info("Constant Buffer '%d' with wrong size '%d' on shader '%s'.", IdBuffer, BufferSize, m_Filename.c_str());  
	l_BufferDescription.BindFlags=D3D11_BIND_CONSTANT_BUFFER;  l_BufferDescription.CPUAccessFlags=0;  
	if(FAILED(l_Device->CreateBuffer(&l_BufferDescription, NULL, &l_ConstantBuffer)))  
	{	
		//Info("Constant buffer '%d' couldn't created on shader '%s'.", IdBuffer, m_Filename.c_str());  
		m_ConstantBuffers.push_back(NULL);   
		return false;  
	} 
	m_ConstantBuffers.push_back(l_ConstantBuffer);  
	return true; 

}

bool CEffectShader::CreateConstantBuffer()
{
	if(CreateConstantBuffer(0,sizeof(CSceneEffectParameters)) && CreateConstantBuffer(1,sizeof(CLightEffectParameters)) && CreateConstantBuffer(2,sizeof(CAnimatedModelEffectParameters)) && CreateConstantBuffer(3,sizeof(CMaterialEffectParameters)))
		return true;

	return false;
}

ID3D11Buffer* CEffectShader::GetConstantBuffer(unsigned int IdBuffer)
{
	if(IdBuffer>=m_ConstantBuffers.size())
		return NULL;
	return m_ConstantBuffers[IdBuffer];
}

