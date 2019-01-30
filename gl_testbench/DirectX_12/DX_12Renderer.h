#pragma once

#include "../Renderer.h"

#include <SDL.h>

class DX_12Renderer : public Renderer
{
public:
	DX_12Renderer();
	~DX_12Renderer();

	Material* makeMaterial(const std::string& name);
	Mesh* makeMesh();
	//VertexBuffer* makeVertexBuffer();
	VertexBuffer* makeVertexBuffer(size_t size, VertexBuffer::DATA_USAGE usage);
	ConstantBuffer* makeConstantBuffer(std::string NAME, unsigned int location);
	//	ResourceBinding* makeResourceBinding();
	RenderState* makeRenderState();
	Technique* makeTechnique(Material* m, RenderState* r);
	Texture2D* makeTexture2D();
	Sampler2D* makeSampler2D();
	std::string getShaderPath();
	std::string getShaderExtension();

	int initialize(unsigned int width = 640, unsigned int height = 480);
	void setWinTitle(const char* title);
	int shutdown();

	void setClearColor(float, float, float, float);
	void clearBuffer(unsigned int);
	//	void setRenderTarget(RenderTarget* rt); // complete parameters
	void setRenderState(RenderState* ps);
	void submit(Mesh* mesh);
	void frame();
	void present();

private:
	SDL_Window * window;
	//TODO: Add all relevant D3D components here

	std::vector<Mesh*> drawList;
	std::unordered_map<Technique*, std::vector<Mesh*>> drawList2;

	bool globalWireframeMode = false;
	float clearColor[4] = { 0,0,0,0 };
};