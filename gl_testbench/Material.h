#pragma once

#include <string>
#include <map>

/* 
 * extend this class with a concrete implementation,
 * public interface should not change...
 */
class Material
{
public:

	struct attribute {
		std::string name;
		std::string type;
	};

	enum class ShaderType { VS = 0, PS = 1, GS = 2, CS = 3 };

	Material() {};
	~Material() {};

	/*
	 * Indicate the shaderName for the shaderType type
	 */
	virtual void setShader(const std::string& shaderName, ShaderType type) = 0;

	// SOMETHING MISSING ABOUT REFLECTION...
	// MAYBE A FUNCTION TO RETURN <NAME,TYPE> FOR EVERY ATTRIBUTE?  

	/*
	 * Returns 0  if compilation suceeded.
	 * Returns -1 if compilation fails.
	 * At least a Vertex and a Fragment shader must be defined.
	*/
	virtual int compileMaterial() = 0;

	virtual void setActive() = 0;

	/*
	 * Updates data for an attribute defined in the shader "ShaderType"
	 * Attribute could be anything, a simple value or a block of values (struct).
	 * returns -1 if attribute does not exist, or material has not been compiled.
	 * returns 0 if the attribute exists in the material.
	 */
	virtual int updateAttribute(
		ShaderType type,
		std::string &attribName,
		void* data,
		unsigned int size) = 0;

	bool isValid() { return _isValid; };
	bool _isValid;
};

