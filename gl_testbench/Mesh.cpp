#include "Mesh.h"
#include "ResourceBinding.h"


Mesh::Mesh()
{
	_drawIndexed = false;
};

/*
	buffer: is a VertexBuffer*
	offset: offset of the first byte in the buffer used when binding
	size: how many elements (how many points, normals, UVs) should be read from this buffer
	inputStream: location of the binding in the VertexShader
*/
void Mesh::addIAVertexBufferBinding(VertexBuffer* buffer, size_t offset, size_t numElements, unsigned int inputStream)
{
	// inputStream is unique (has to be!) for this Mesh
	geometryBuffers[inputStream] = { numElements, offset, buffer };
};

void Mesh::bindIAVertexBuffer(unsigned int location)
{
	// no checking if the key is valid...TODO
	const VertexBufferBind& vb = geometryBuffers[location];
	vb.buffer->bind(vb.offset,vb.buffer->getSize(),location);
}

/*
void Mesh::setTechnique(Material* m, RenderState* s, const std::vector<ResourceBinding*>* bindings = nullptr) {
	Technique* t = new Technique();
	if (bindings)
	{
		for (auto b : *bindings)
		{
			// copy binding
			t->bindings.push_back(new ResourceBinding(*b));
		};
	};
	t->material = m;
	this->technique = t;
};
*/

Mesh::~Mesh()
{
}
