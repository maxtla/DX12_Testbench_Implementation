#pragma once
#include <vector>
#include "RenderState.h"

class RenderStateGL : public RenderState
{
public:
	RenderStateGL();
	~RenderStateGL();

	void setDepthTest(bool);
	void setDepthWrite(bool);
	void setWireFrame(bool);
};

