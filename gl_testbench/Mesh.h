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
	 format follows this convention (with a fixed width description)
	 tn:tn:tn
	 t is the type: float(f), int(i), uint(u), double(d)
	 n is the number of elements: 1,2,3,4
	 s is the semantics: position(p), normal(n), uv(u), undefined(x)
	 For example, format for vec3 (position) and vec2 (uv) would be: "f3p:f2u"
	 Internally the function will determine the layout, in this case interleaved.

	 data is a pointer to the bytes (expected contiguous data, with no padding).
	 On the concrete class (MeshGL, MeshVK, ..., MeshDX11, MeshDX12) the appropriate calls will be made
	 If no interleaved data is desired, then simply call this function multiple times, in order.
	 After calling this function, the data is expected to be copied inside the Mesh instance or stored 
	 in a GPU buffer...
	 */
	virtual void addIAVertexBuffer(std::string format,	void* data, unsigned long count, DATA_USAGE usage = STATIC) = 0;	
	virtual void addIAIndexBuffer(std::vector<unsigned int> indices) = 0;
};
