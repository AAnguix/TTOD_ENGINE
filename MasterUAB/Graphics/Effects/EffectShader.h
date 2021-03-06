#ifndef _EFFECTSHADER_H
#define _EFFECTSHADER_H

#include "Utils\Named.h"
#include <d3d11.h>
#include <d3dcommon.h>
#include <vector>

class CEffectShader : public CNamed
{

protected:

	std::vector<ID3D11Buffer *> m_ConstantBuffers;
	std::string	m_Filename;
	std::string m_Preprocessor;
	std::string m_ShaderModel;
	std::string m_EntryPoint;
	std::vector<std::string>  m_PreprocessorMacros;
	D3D10_SHADER_MACRO *m_ShaderMacros;
	
	virtual void Destroy();
	void CreateShaderMacro();
	bool LoadShaderExtended(const std::string &ShaderName, const std::string &FileName, const std::string &EntryPoint, const std::string &ShaderModel, ID3DBlob **BlobOut, size_t EffectsStateCode);
	bool LoadShader(const std::string &Filename, const std::string &EntryPoint, const std::string &ShaderModel, ID3DBlob **BlobOut);

	bool CreateConstantBuffer(int IdBuffer, unsigned int BufferSize);
	bool CreateConstantBuffer();

	virtual void SetConstantBuffer(unsigned int IdBuffer, void *ConstantBuffer) = 0;
public:

	CEffectShader(const CXMLTreeNode &TreeNode);
	virtual ~CEffectShader();
	virtual bool Load(size_t EffectsStateCode) = 0;
	virtual bool Reload();

	ID3D11Buffer * GetConstantBuffer(unsigned int IdBuffer);

private:
	void SplitString(const std::string String,char Separator,std::vector<std::string> &Array);

};

#endif
