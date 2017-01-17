#include <string>
#include <SDL_keyboard.h>
#include <SDL_events.h>
#include <type_traits> 

//#include "Material.h"
#include "Technique.h"
#include "Renderer.h"
#include "Mesh.h"

using namespace std;
Renderer* renderer;

// flat scene
vector<Mesh*> scene;
vector<Material*> materials;

// forward
void updateScene();
void renderScene();

typedef union { 
	struct { float x, y, z, w; };
	struct { float r, g, b, a; };
} float4;

typedef union { 
	struct { float x, y; };
	struct { float u, v; };
} float2;


void run() {
	SDL_Event windowEvent;
	while (true)
	{
		if (SDL_PollEvent(&windowEvent))
		{
			if (windowEvent.type == SDL_QUIT) break;
			if (windowEvent.type == SDL_KEYUP && windowEvent.key.keysym.sym == SDLK_ESCAPE) break;
		}
		renderer->clearBuffer(CLEAR_BUFFER_FLAGS::COLOR | CLEAR_BUFFER_FLAGS::DEPTH);
		updateScene();
		renderScene();
		renderer->swapBuffers();
	}
	renderer->shutdown();
}

/*
 update positions of triangles in the screen
 changing their world matrix
*/
void updateScene()
{
	/*
	    For each mesh in scene list, update their position 
	*/
	for (auto m : scene)
	{
		// update m->parts[0]->worldMatrix to a random value!
		// no coherency considered.
	}
	return;
};


void renderScene()
{
	renderer->clearBuffer(CLEAR_BUFFER_FLAGS::COLOR); // | CLEAR_BUFFER_FLAGS::DEPTH);
	for (auto m : scene)
	{
		renderer->submit(m);
	}
	renderer->frame();
	renderer->present();
}

int initialiseTestbench()
{
	// triangle geometry:
	float4 triPos[3] = { { 0.0f,  0.5f, 0.0f, 1.0f },{ 0.5f, -0.5f, 0.0f, 1.0f },{ -0.5f, -0.5f, 0.0f, 1.0f } };
	float4 triNor[3] = { { 1.0f,  0.0f, 0.0f, 0.0f },{ 1.0f, 0.0f, 0.0f, 0.0f },{ 1.0f, 0.0f, 0.0f, 0.0f } };
	float2 triUV[3] = { { 0.0f,  0.0f },{ 1.0f, 0.0f },{ 0.0f, 1.0f } };

	// load Materials.
	// for (int i = 0; i < 1; i++)
	{
		// set material name from text file?
		Material* m = renderer->makeMaterial();
		std::string shaderPath = renderer->getShaderPath();
		m->setShader(shaderPath + std::string("VertexShader.glsl"), Material::ShaderType::VS);
		m->addDefine("#define POSITION 0\n #define NORMAL 1\n #define TEXTCOORD 2\n", Material::ShaderType::VS);

		m->setShader(shaderPath + std::string("FragmentShader.glsl"), Material::ShaderType::PS);
		m->addDefine("#define NORMAL 1\n #define TEXTCOORD 2\n", Material::ShaderType::PS);

		std::string err;
		m->compileMaterial(err);

		materials.push_back(m);
	}

	// basic technique
	Technique* technique = new Technique();
	technique->material = materials[0];

	// Create a mesh array, to push API interaction each mesh will have only ONE meshPart.
	for (int i = 0; i < 1; i++) {
		Mesh* m = renderer->makeMesh();

		VertexBuffer* pos = renderer->makeVertexBuffer();
		pos->setData(triPos, sizeof(triPos), VertexBuffer::STATIC);
		pos->bind(0, sizeof(triPos), POSITION);

		VertexBuffer* nor = renderer->makeVertexBuffer();
		nor->setData(triNor, sizeof(triNor), VertexBuffer::STATIC);
		nor->bind(0, sizeof(triNor), NORMAL);

		VertexBuffer* uvs = renderer->makeVertexBuffer();
		uvs->setData(triUV, sizeof(triUV), VertexBuffer::STATIC);
		uvs->bind(0, sizeof(triUV), TEXTCOORD);

		// C++++++ way for getting the number of elements in an array
		constexpr auto numberOfElements = std::extent<decltype(triPos)>::value;

		m->addIAVertexBufferBinding(pos, 0, numberOfElements, POSITION);
		m->addIAVertexBufferBinding(nor, 0, numberOfElements, NORMAL);
		m->addIAVertexBufferBinding(uvs, 0, numberOfElements, TEXTCOORD);

		m->technique = technique;

		scene.push_back(m);
	}
	return 0;
}

void finishApplication() {
};

int main(int argc, char *argv[])
{
	renderer = Renderer::makeRenderer(Renderer::BACKEND::GL45);
	renderer->initialize();
	renderer->setClearColor(0.0, 0.0, 0.0, 1.0);
	initialiseTestbench();
	run();
	finishApplication();
	return 0;
};
