#pragma once
#include "Material.h"
#include <GL/glew.h>


class MaterialGL :
	public Material
{
public:
	MaterialGL();
	~MaterialGL();

	void setShader(const std::string& shaderName, ShaderType type);

	int compileMaterial(std::string& errString);
	int enable();
	void disable();

private:
	// map from ShaderType to GL_VERTEX_SHADER, ...
	GLuint mapShaderEnum[4];
	std::string shaderNames[4];
	// opengl shader object
	GLuint shaderObject[4] = { 0,0,0,0 };
	// TODO: change to PIPELINE
	// opengl program object
	GLuint program = 0;
};

