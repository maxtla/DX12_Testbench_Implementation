#pragma once
#include <vector>
#include "ResourceBinding.h"
#include "Material.h"
#include "RenderState.h"

class Technique
{
public:
	Technique();
	~Technique();
	//void setMaterial(Material* m);
	//void setRenderState(RenderState* rs);
	//void setBindings(std::vector<ResourceBinding*>* rb);

	Material* material;
	RenderState* renderState;
	std::vector<ResourceBinding*>* bindings;
};

