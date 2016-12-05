#pragma once
#include "RenderTarget.h"
#include "DepthStencil.h"
#include "PipelineState.h"
#include "DrawInfo.h"
#include "Mesh.h"

class Renderer {
public:
	enum class BACKEND { GL45, VULKAN, DX11, DX12 };
	enum class CLEAR_BUFFER_FLAGS { COLOR = 1, DEPTH = 2, STENCIL = 4 };

	static Renderer *makeRenderer(BACKEND backend);

	virtual int initialize(unsigned int width = 800, unsigned int height = 600) = 0;
	virtual void swapBuffers() = 0;
	virtual int shutdown() = 0;

	virtual void setClearColor(float, float, float, float) = 0;
	virtual void clearBuffer(CLEAR_BUFFER_FLAGS) = 0;
	virtual void setRenderTarget(RenderTarget* rt) = 0; // complete parameters
	// can be partially overriden by a specific Technique.
	virtual void setRenderState(RenderState* ps) = 0;
	virtual void draw(Mesh* mesh, DrawInfo* data = nullptr) = 0;
};
