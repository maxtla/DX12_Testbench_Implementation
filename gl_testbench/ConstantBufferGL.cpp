#include "ConstantBufferGL.h"



ConstantBufferGL::ConstantBufferGL() 
{
	_handle = 0;
}


ConstantBufferGL::~ConstantBufferGL()
{
	glDeleteBuffers(1, &_handle);
	_handle = 0;
}

// this allows us to not know in advance the type of the receiving end, vec3, vec4, etc.
void ConstantBufferGL::setData(const void* data, size_t size, unsigned int location)
{
	if (_handle == 0) {
		glGenBuffers(1, &_handle);
		glBindBuffer(GL_UNIFORM_BUFFER, _handle);
		glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_STATIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, location, _handle);
	}
	glBindBuffer(GL_UNIFORM_BUFFER, _handle);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, size, data);
}

