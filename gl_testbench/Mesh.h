#pragma once
#include <vector>
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

	Technique* technique;
	//void setTechnique(Material* m, RenderState* s, const std::vector<ResourceBinding*>* bindings = nullptr);

	std::vector<Buffer*> vertexBuffers;
	Buffer* indexBuffer;
	Transform* worldMatrix;
};

