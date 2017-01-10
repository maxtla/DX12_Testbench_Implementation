#pragma once
#include <vector>
#include <string>
#include "Technique.h"

class Buffer;
class Transform;

/*
 * To render a mesh, use the renderer function.
 */

class Mesh
{
public:
	Mesh();
	~Mesh();
	// vertex buffers usage hints
	enum DATA_USAGE { STATIC, DYNAMIC, DONTCARE };
	Technique* technique;

	//std::vector<Buffer*> vertexBuffers;
	//Buffer* indexBuffer;
	Transform* worldMatrix;

	/*
	 On the concrete class (MeshGL, MeshVK, ..., MeshDX11, MeshDX12) the appropriate calls will be made
	 If no interleaved data is desired, then simply call this function multiple times, in order.
	 After calling this function, the data is expected to be copied inside the Mesh instance or stored 
	 in a GPU buffer...
	 */
	virtual void addIAVertexBuffer(void* data, unsigned long count, DATA_USAGE usage = STATIC) = 0;	
	virtual void addIAIndexBuffer(std::vector<unsigned int> indices) = 0;
};
