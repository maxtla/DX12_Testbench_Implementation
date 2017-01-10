#include "MeshGL.h"



MeshGL::MeshGL()
{
}


MeshGL::~MeshGL()
{
}

void MeshGL::addIAVertexBuffer(std::string format, void* data, unsigned long count, DATA_USAGE usage = STATIC)
{
	// create one VAO
	GLuint newVao;
	glGenVertexArrays(1, &newVao);


}
void MeshGL::addIAIndexBuffer(std::vector<unsigned int> indices)
{

}
