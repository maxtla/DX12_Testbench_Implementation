#pragma once
#include <unordered_map>
#include "IA.h"
#include "VertexBuffer.h"
//#include "Technique.h"

class Transform;
class Technique;

class Mesh
{
public:
	Mesh();
	~Mesh();
	// vertex buffers usage hints
	Technique* technique;
	Transform* worldMatrix;
	bool _drawIndexed;

	struct VertexBufferBind {
		size_t numElements, offset;
		VertexBuffer* buffer;
	};
	// array of buffers with locations (binding points in shaders)
	void addIAVertexBufferBinding(VertexBuffer* buffer, size_t offset, size_t size, unsigned int inputStream);
	void bindIAVertexBuffer(unsigned int location);
	std::unordered_map<unsigned int, VertexBufferBind> geometryBuffers;
};
