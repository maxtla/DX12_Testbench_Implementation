#pragma once

#include <vector>
#include <memory>
#include "Pass.h"

class Technique
{
public:
	Technique();
	~Technique();

	// passess can be reused
	std::vector<std::shared_ptr<Pass>> passes;
};

