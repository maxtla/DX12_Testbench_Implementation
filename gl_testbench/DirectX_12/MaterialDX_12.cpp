#pragma comment(lib, "D3DCompiler.lib")

#include "MaterialDX_12.h"
#include <d3dcompiler.h>

MaterialDX_12::MaterialDX_12(const std::string & name)
{
	isValid = false;
	this->name = name;
}

MaterialDX_12::~MaterialDX_12()
{
	for (auto cb : constantBuffers)
	{
		if (cb.second != nullptr)
		{
			delete(cb.second);
			cb.second = nullptr;
		}
	}
}

void MaterialDX_12::setShader(const std::string & shaderFileName, ShaderType type)
{
	if (shaderFileNames.find(type) != shaderFileNames.end())
		removeShader(type);

	shaderFileNames[type] = shaderFileName;
}

void MaterialDX_12::removeShader(ShaderType type)
{
	if (shaderFileNames.find(type) == shaderFileNames.end())
	{
		return;
	}
	else if (0)
	{
		// delete macros from previous shaders
	}
}

void MaterialDX_12::setDiffuse(Color c)
{
}

int MaterialDX_12::compileMaterial(std::string & errString)
{
	// cleanup from previous calls
	removeShader(ShaderType::VS);
	removeShader(ShaderType::PS);

	// split defines VS
	_findMacros(ShaderType::VS, VS_Macros);

	HRESULT hr;

	ID3DBlob* vertexBlob;
	hr = D3DCompileFromFile(
		LPCWSTR(shaderFileNames[ShaderType::VS].c_str()),
		VS_Macros,
		nullptr,
		"VS_main",
		"vs_5_0",
		0,
		0,
		&vertexBlob,
		nullptr
	);
	
	if (FAILED(hr))
	{
		errString = hr;
		return -1;
	}

	// split defines PS
	_findMacros(ShaderType::PS, PS_Macros);

	ID3DBlob* pixelBlob;
	hr = D3DCompileFromFile(
		LPCWSTR(shaderFileNames[ShaderType::PS].c_str()),
		PS_Macros,
		nullptr,
		"PS_main",
		"ps_5_0",
		0,
		0,
		&pixelBlob,
		nullptr
	);

	if (FAILED(hr))
	{
		errString = hr;
		return -1;
	}

	// Create PSO here?

	return 0;
}

void MaterialDX_12::addConstantBuffer(std::string name, unsigned int location)
{
}

void MaterialDX_12::updateConstantBuffer(const void * data, size_t size, unsigned int location)
{
}

int MaterialDX_12::enable()
{
	if (!isValid)
		return -1;

	for (auto cb : constantBuffers)
		cb.second->bind(this);

	return 0;
}

void MaterialDX_12::disable()
{

}

void MaterialDX_12::_findMacros(ShaderType type, D3D_SHADER_MACRO* macros)
{
	auto shader = shaderDefines.find(type);
	macros = new D3D_SHADER_MACRO[shader->second.size() + 1];
	int count = 0;
	for (auto it = shader->second.begin(); it != shader->second.end(); it++)
	{
		size_t postDefine = (*it).find(' ');
		size_t postName = (*it).find(' ', postDefine + 1);
		std::string macroName = (*it).substr(postDefine + 1, postName - postDefine);
		std::string macroDefinition = (*it).substr(postName + 1);
		D3D_SHADER_MACRO macro;
		macro.Name = LPCSTR(macroName.c_str());
		macro.Definition = LPCSTR(macroDefinition.c_str());
		macros[count++] = macro;
	}
	macros[count] = { NULL, NULL };
}
