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
			delete cb.second;
			cb.second = nullptr;
		}
	}
	SafeRelease(&m_vertexShaderCode);
	SafeRelease(&m_pixelShaderCode);
}

void MaterialDX_12::setShader(const std::string & shaderFileName, ShaderType type)
{
	shaderFileNames[type] = shaderFileName;
}

void MaterialDX_12::removeShader(ShaderType type)
{
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
	_findMacros(ShaderType::VS, &VS_Macros);

	HRESULT hr;

	//Conversion to wstring
	std::wstring path = std::wstring(shaderFileNames[ShaderType::VS].begin(), shaderFileNames[ShaderType::VS].end());

	hr = D3DCompileFromFile(
		LPCWSTR(path.c_str()),
		VS_Macros,
		nullptr,
		"VS_main",
		"vs_5_0",
		0,
		0,
		&this->m_vertexShaderCode,
		nullptr
	);
	
	if (FAILED(hr))
	{
		errString = hr;
		return -1;
	}

	// split defines PS
	_findMacros(ShaderType::PS, &PS_Macros);

	//Conversion to wstring
	path = std::wstring(shaderFileNames[ShaderType::PS].begin(), shaderFileNames[ShaderType::PS].end());

	hr = D3DCompileFromFile(
		LPCWSTR(path.c_str()),
		PS_Macros,
		nullptr,
		"PS_main",
		"ps_5_0",
		0,
		0,
		&this->m_pixelShaderCode,
		nullptr
	);

	if (FAILED(hr))
	{
		errString = hr;
		return -1;
	}

	// Create Shader objects here

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
	return 0;
}

void MaterialDX_12::disable()
{

}

void MaterialDX_12::_findMacros(ShaderType type, D3D_SHADER_MACRO** macros)
{
	auto shader = shaderDefines.find(type);
	int count = 0;

	std::vector<std::string> defines;
	std::vector<std::string> values;
	// Parse set for all the different defines

	std::string toParse = shader->second.begin()->c_str();

	size_t i = 0;
	while (i != toParse.size() - 1) 
	{
		size_t postDefine = toParse.find(' ', i);
		size_t postName = toParse.find(' ', postDefine + 1);
		size_t lineTermination = toParse.find('\n', postName);
		i = lineTermination;

		defines.push_back(toParse.substr(postDefine + 1, postName - postDefine - 1));
		values.push_back(toParse.substr(postName + 1, lineTermination - postName - 1));
	}

	size_t size = defines.size();

	(*macros) = new D3D_SHADER_MACRO[size + 1];

	for (size_t i = 0; i < size; i++)
	{
		(*macros)[i] = { (LPCSTR)defines[i].c_str(), (LPCSTR)values[i].c_str() };
	}
	(*macros)[size] = { NULL, NULL };
}
