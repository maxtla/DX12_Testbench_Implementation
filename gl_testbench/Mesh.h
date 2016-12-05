#pragma once

#include "MeshPart.h"
#include <vector>

/*
 * A mesh is just a container of mesh parts.
 * Material information is in the MeshParts and
 * can be different.
 * To render a mesh, use the renderer function.
 */

class Mesh
{
public:
	Mesh();
	~Mesh();
	std::vector<MeshPart*> parts;
};

