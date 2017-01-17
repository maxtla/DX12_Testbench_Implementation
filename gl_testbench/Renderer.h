#pragma once
#include "RenderTarget.h"
#include "DepthStencil.h"
#include "RenderState.h"
#include "DrawInfo.h"
#include "Technique.h"

#include <unordered_map>
#include <Windows.h>

class Mesh;
class VertexBuffer;

//CRITICAL_SECTION protectHere;
//#define LOCK EnterCriticalSection(&protectHere)
//#define UNLOCK LeaveCriticalSection(&protectHere)

namespace CLEAR_BUFFER_FLAGS {
	static const int COLOR = 1;
	static const int DEPTH = 2;
	static const int STENCIL = 4;
};

class Renderer {
public:
	enum class BACKEND { GL45, VULKAN, DX11, DX12 };

	/*
	Return concrete objects of the BACKEND
	*/
	static Renderer* makeRenderer(BACKEND backend);
	virtual Material* makeMaterial() = 0;
	virtual Mesh* makeMesh() = 0;
	virtual VertexBuffer* makeVertexBuffer() = 0;
	virtual ResourceBinding* makeResourceBinding() = 0;
	virtual RenderState* makeRenderState() = 0;
	virtual std::string getShaderPath() = 0;

	Renderer() { /*InitializeCriticalSection(&protectHere);*/ };
	virtual int initialize(unsigned int width = 800, unsigned int height = 600) = 0;
	virtual void swapBuffers() = 0;
	virtual void present() = 0;
	virtual int shutdown() = 0;

	virtual void setClearColor(float, float, float, float) = 0;
	virtual void clearBuffer(unsigned int) = 0;
	virtual void setRenderTarget(RenderTarget* rt) = 0; // complete parameters
	// can be partially overriden by a specific Technique.
	virtual void setRenderState(RenderState* ps) = 0;
	// submit work (to render) to the renderer.
	virtual void submit(Mesh* mesh) = 0;
	virtual void frame() = 0;
	
	Technique* createTechnique();
	Technique* getTechnique(unsigned int techniqueId);
	std::unordered_map<unsigned int, Technique*> techniques;
	
	// very naive reference counting for VOID*, so whatever you want to put here....
	// DO NOT PUT Dynamic arrays in here...
	//void ref(void* ptr) { 
	//	LOCK; 
	//	if (refCount.count(ptr) == 0) 
	//		refCount[ptr] = 1; 
	//	else refCount[ptr]+=1; 
	//	UNLOCK; 
	//};
	//void unref(void* ptr) { 
	//	LOCK; 
	//	if (refCount.count(ptr) == 1) {
	//		refCount[ptr]--; 
	//		if (refCount[ptr] == 0)
	//			delete ptr;
	//	}
	//	UNLOCK; 
	//};

	BACKEND IMPL;
	//std::unordered_map<void*, unsigned int> refCount;
};
