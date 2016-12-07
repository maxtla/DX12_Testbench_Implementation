#include "Mesh.h"



Mesh::Mesh()
{
}


Mesh::~Mesh()
{
	for (MeshPart* part : parts) {
		if (part != nullptr)
			delete part;
	}
}
