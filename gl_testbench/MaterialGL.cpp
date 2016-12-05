#include "MaterialGL.h"
#include <fstream>
#include <streambuf>

typedef unsigned int uint;

MaterialGL::MaterialGL()
{ 
	isValid = false;
	mapShaderEnum[(uint)ShaderType::VS] = GL_VERTEX_SHADER;
	mapShaderEnum[(uint)ShaderType::PS] = GL_FRAGMENT_SHADER;
	mapShaderEnum[(uint)ShaderType::GS] = GL_GEOMETRY_SHADER;
	mapShaderEnum[(uint)ShaderType::CS] = GL_COMPUTE_SHADER;
};

MaterialGL::~MaterialGL() 
{ 
	// most basic for now.
	for (auto shaderObject : shaderObject) {
		glDeleteShader(shaderObject);
	};
	glDeleteProgram(program);
}

void MaterialGL::setShader(const std::string& shaderName, ShaderType type)
{
	// set new name, and delete old shader if it is valid.
	// does not compile...
	if (shaderObject[(uint)type] != 0)
	{
		glDeleteShader(shaderObject[(uint)type]);
		shaderObject[(uint)type] = 0;
	}
	shaderNames[(uint)type] = shaderName;
	if (isValid) {
		glDeleteProgram(program);
		program = 0;
	}
	isValid = false;
}

int MaterialGL::compileMaterial(std::string& errString)
{
	for (int i = 0; i < 4; i++)
	{
		if (shaderNames[i].length() != 0)
		{
			// detach and delete if there is a shader defined before.
			if (shaderObject[i] != 0 && program != 0) {
				glDetachShader(program, shaderObject[i]);
				glDeleteShader(shaderObject[i]);
			}

			// try to compile this file.
			std::ifstream shaderFile(shaderNames[i]);
			std::string shaderText;
			if (shaderFile.is_open()) {
				shaderText = std::string((std::istreambuf_iterator<char>(shaderFile)), std::istreambuf_iterator<char>());
				shaderFile.close();
			}
			else {
				errString = "Cannot find file: " + shaderNames[i];
				return -1;
			}
			shaderObject[i] = glCreateShader(mapShaderEnum[i]);
			// make a double pointer (only valid here)
			const char* shaderTextPtr = shaderText.c_str();
			// ask GL to load this
			glShaderSource(shaderObject[i], 1, &shaderTextPtr, nullptr);
			// ask GL to compile it
			glCompileShader(shaderObject[i]);
			// handle error: TODO
		}
	}

	// try to link the program
	//link shader program (connect vs and ps)
	if (program != 0)
		glDeleteProgram(program);
	program = glCreateProgram();
	for (int i = 0; i < 4; i++) {
		if (shaderObject[i] != 0)
			glAttachShader(program, shaderObject[i]);
	}
	glLinkProgram(program);
	return 0;
}

int MaterialGL::enable() {
	if (program == 0)
		return -1;
	glUseProgram(program);
	return 0;
}

void MaterialGL::disable() {
	glUseProgram(0);
}

//int MaterialGL::updateAttribute(
//	ShaderType type,
//	std::string &attribName,
//	void* data,
//	unsigned int size)
//{
//	return 0;
//}
