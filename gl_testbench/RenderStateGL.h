#pragma once
#include <vector>
#include "RenderState.h"

class RenderStateGL : public RenderState
{
public:
	RenderStateGL();
	~RenderStateGL();

	// enable or disable things:
	void enableDepth(bool value);

	// set specific values:

	void setState(State& state);
};

