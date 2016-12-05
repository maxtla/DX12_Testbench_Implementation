#pragma once

class Technique;
class BufferResource;
class Transform;

class MeshPart
{
public:
	enum class MESH_BUFF { VTX = 0, IDX = 1 };
	MeshPart();
	~MeshPart();

	virtual void setVertexBuffer(BufferResource*, MESH_BUFF type) = 0;
	virtual void setTechnique(Technique*) = 0;

	// public...
	Technique* technique;
	BufferResource* vertexBuffer;
	BufferResource* indexBuffer;

	// helpers for testing
	virtual void setTriangleMesh() = 0;
	virtual void setQuadMesh() = 0;

	// mesh properties
	Transform* worldMatrix;
};

