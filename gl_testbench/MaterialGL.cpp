#include "MaterialGL.h"

using namespace globjects;
using namespace gl;

// allocate space for it.
std::map<MaterialGL::ShaderType, gl::GLenum> MaterialGL::shaderTypeMap;

MaterialGL::MaterialGL()
{ 
	program = new Program();
	program->ref();
	_isValid = false;
	shaderTypeMap[ShaderType::VS] = GL_VERTEX_SHADER;
	shaderTypeMap[ShaderType::PS] = GL_FRAGMENT_SHADER;
	shaderTypeMap[ShaderType::GS] = GL_GEOMETRY_SHADER;
	shaderTypeMap[ShaderType::CS] = GL_COMPUTE_SHADER;
};

MaterialGL::~MaterialGL() 
{ 
	program->unref();
}

void MaterialGL::setShader(const std::string& shaderName, ShaderType type)
{
	unsigned int idx = (unsigned int)type;
	if (shaders[idx])
		shaders[idx]->unref();
	shaders[idx] = Shader::fromFile(shaderTypeMap[type], shaderName);
}

int MaterialGL::compileMaterial()
{
	if (program->isLinked())
	{
		program->release();
		program->destroy();
	}
	
	for (auto& shader : shaders)
	{
		shader->compile();
		if (shader->isCompiled())
			program->attach(shader);
	}
	program->link();
	return 0;
}

void MaterialGL::setActive() {
	if (program && _isValid && program->isLinked())
		program->use();
}

int MaterialGL::updateAttribute(
	ShaderType type,
	std::string &attribName,
	void* data,
	unsigned int size)
{
	return 0;
}
