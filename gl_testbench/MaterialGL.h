#pragma once
#include "Material.h"
#include <globjects/Program.h>
#include <globjects/Shader.h>
#include <globjects/globjects.h>
#include <glbinding/gl/gl.h>
#include <map>

class MaterialGL :
	public Material
{
public:
	MaterialGL();
	~MaterialGL();
	void setShader(const std::string& shaderName, ShaderType type);
	int compileMaterial();
	void setActive();
	int updateAttribute(
		ShaderType type,
		std::string &attribName,
		void* data,
		unsigned int size);
private:
	globjects::Program* program;
	globjects::Shader* shaders[4];
	static std::map<ShaderType, gl::GLenum> shaderTypeMap;
};

