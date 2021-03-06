#pragma once
#include "../Material.h"
#include <vector>
#include "ConstantBufferDX_12.h"
#include <d3dcommon.h>

class MaterialDX_12 : public Material
{
public:
	MaterialDX_12(const std::string& name, ID3D12GraphicsCommandList * pCommandList);
	~MaterialDX_12();

	// set shader name, DOES NOT COMPILE
	void setShader(const std::string& shaderFileName, ShaderType type);

	// removes any resource linked to shader type
	void removeShader(ShaderType type);

	void setDiffuse(Color c);

	/*
	* Compile and link all shaders
	* Returns 0  if compilation/linking succeeded.
	* Returns -1 if compilation/linking fails.
	* Error is returned in errString
	* A Vertex and a Fragment shader MUST be defined.
	* If compileMaterial is called again, it should RE-COMPILE the shader
	* In principle, it should only be necessary to re-compile if the defines set
	* has changed.
	*/
	int compileMaterial(std::string& errString);

	// this constant buffer will be bound every time we bind the material
	void addConstantBuffer(std::string name, unsigned int location);

	// location identifies the constant buffer in a unique way
	void updateConstantBuffer(const void* data, size_t size, unsigned int location);

	// activate the material for use.
	int enable();

	// disable material
	void disable();

	std::map<unsigned int, ConstantBufferDX_12*> constantBuffers;

	std::pair<ID3DBlob*, ID3DBlob*> GetShaderCodePtrs() { return std::pair< ID3DBlob*, ID3DBlob*>(m_vertexShaderCode, m_pixelShaderCode); }
private:
	std::string name;
	D3D_SHADER_MACRO* VS_Macros = NULL;
	D3D_SHADER_MACRO* PS_Macros = NULL;
	
	ID3DBlob * m_vertexShaderCode	 = NULL;
	ID3DBlob * m_pixelShaderCode		 = NULL;

	ID3D12GraphicsCommandList * m_commandList = nullptr;
	float m_data[4];
private:
	void _findMacros(ShaderType type, D3D_SHADER_MACRO** macros);
};
