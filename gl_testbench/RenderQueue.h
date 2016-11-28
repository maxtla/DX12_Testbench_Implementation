#pragma once
class RenderQueue
{
public:
	// queue related commands:
	// ----------------------
	// 
	// startQueue(): empty queue of commands
	//
	// finishQueue(): tell Renderer that all the commands have been issued.
	// On a single threaded implementation the Renderer would pick the commands up,
	// do whatever he wants, and submit the real commands to the GPU.
	// On a multi threaded, maybe the queue could have the indirect device/context and
	// make the submissions, but it would obviously not allow for any ordering optimisation
	// to take place (materials, z-order, etc)
	//
	// =====================================================
	//
	// rendering commands:
	//
	// setClearColor(float,float,float,float)
	// clearBuffer(FLAGS): could be backbuffer, stencil, depth...
	// setDepthStencilBuffer(bitdepth1, bitdepth2)
	//
	// setRenderTarget(RenderTarget* t, unsigned int index=0):
	// support for Deferred ??
	//
	// setFixedState(State): adjust fixed state values
	// -- cull, winding, depth test, msaa, anything..., write only Red, etc.
	// 
	// draw(Mesh): 
	// -- sets Material for this mesh (shader and input layout)
	// -- deals with indexed and non-indexed.
	// -- sets all buffers ??? or we have a setbuffer command in the API?
	//
	// setBuffer(Buffer)??? this could be done by the draw(Mesh)
	// 
	// drawInstanced(Mesh, InstanceData?)
	//

	// no scissors or viewRect commands.
};

