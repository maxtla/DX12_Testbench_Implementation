#pragma once

#include "MeshPart.h"

class Mesh
{
public:
	Mesh();
	~Mesh();

	MeshPart* parts;
	unsigned int partsCount;
};

