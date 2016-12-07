#pragma once

#include <vector>
#include <memory>

class Technique;
class Buffer;
class Transform;

class MeshPart
{
public:
	enum class MESH_BUFF { VTX = 0, IDX = 1 };
	~MeshPart();
	MeshPart();
	
	// a technique can be reused by many meshes
	std::shared_ptr<Technique> technique;
	// assign with: technique = make_shared<Technique>();

	std::vector<std::shared_ptr<Buffer>> vertexBuffers;
	std::shared_ptr<Buffer> indexBuffer;

	// helpers for testing
	//virtual void setTriangleMesh() = 0;
	//virtual void setQuadMesh() = 0;
	// mesh properties
	Transform* worldMatrix;
};

