#pragma once
#include <gl\glew.h>
#include "Mesh.h"


class MeshGL :
	public Mesh
{
public:
	MeshGL();
	~MeshGL();
	void addIAVertexBuffer(std::string format, void* data, unsigned long count, DATA_USAGE usage = STATIC);
	void addIAIndexBuffer(std::vector<unsigned int> indices);

private:
// the number of VAOs
	std::vector<GLuint> VAOs;
	std::vector<GLuint> VBOs;

};

