#include "Technique.h"

#include <iostream>


Technique::Technique()
{
}

Technique::~Technique()
{
	std::cout << "destroy technique" << std::endl;
}

//void Technique::setMaterial(Material* m) { material = m; }
//void Technique::setRenderState(RenderState* rs) { renderState = rs; }
//void Technique::setBindings(std::vector<ResourceBinding*>* rb) { bindings = rb; }
