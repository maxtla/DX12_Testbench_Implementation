#include "Mesh.h"
#include "ResourceBinding.h"


Mesh::Mesh()
{
};

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
