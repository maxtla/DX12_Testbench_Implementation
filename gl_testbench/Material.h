#pragma once

#include <string>

/* 
 * extend this class with a concrete implementation,
 * public interface should not change...
 * 
 * A material represents the programmable part of the pipeline:
 * Vertex, Geometry (wont be used), Fragment and Compute Shaders (wont be used)
 * Only need to support Vertex and Fragment for now.
 * Any extra functionality should be added to a concrete subclass, including instrospection
 * to figure out input slots for the VS.
 */
class Material
{
public:

	enum class ShaderType { VS = 0, PS = 1, GS = 2, CS = 3 };

	Material() : isValid(false) {};
	~Material() {};

	// setShader (does not compile)
	virtual void setShader(const std::string& shaderFileName, ShaderType type) = 0;

	/*
	 * Compile and link all shaders
	 * Returns 0  if compilation/linking suceeded.
	 * Returns -1 if compilation/linking fails.
	 * Error can be collected in errString
	 * At least a Vertex and a Fragment shader must be defined.
	*/
	virtual int compileMaterial(std::string& errString) = 0;

	// activate the material for use.
	virtual int enable() = 0;
	// disable material
	virtual void disable() = 0;

	//virtual int updateAttribute(ShaderType, std::string&, void*, unsigned int) = 0;
	//virtual int setAttributePtr(ShaderType, std::string&, void*) = 0; 

	bool isValid;
};

