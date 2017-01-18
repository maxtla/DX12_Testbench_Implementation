#pragma once
#include <GL/glew.h>
#include "ConstantBuffer.h"

class ConstantBufferGL : public ConstantBuffer
{
public:
	ConstantBufferGL();
	~ConstantBufferGL();
	void setData(const void* data, size_t size, unsigned int location);

private:
	GLuint _handle;
};

