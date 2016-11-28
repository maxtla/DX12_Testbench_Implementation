#pragma once
#include "RenderQueue.h"

class Renderer {
public:
	enum class BACKEND { GL45, VULKAN, DX11, DX12 };
	static Renderer *makeRenderer(BACKEND backend);

	// create a unique Render Command Queue.
	// thread-safe method
	virtual RenderQueue* getRenderCommandQueue() = 0;

	virtual bool waitForQueues() = 0;
	// all command queues have been finished by all threads
	// and it is safe to process the commands.

	virtual void executeCommandQueues() = 0;
	// execute all command queues in whatever order suits...

	virtual int initialize(unsigned int width = 800, unsigned int height = 600) = 0;
	virtual void swapBuffers() = 0;
	virtual int shutdown() = 0;
};
