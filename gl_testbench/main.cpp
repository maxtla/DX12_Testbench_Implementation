#include <string>
#include <SDL_keyboard.h>
#include <SDL_events.h>
#include <type_traits> 

//#include "Material.h"
#include "Technique.h"
#include "Renderer.h"
#include "Mesh.h"
#include "Transform.h"

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
		updateScene();
		renderScene();
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
	float translation[4] = { 0.0,0.0,0.0,0.0 };

	for (auto m : scene)
	{
		translation[0] += 0.1;
		m->txBuffer->setData(translation, sizeof(translation), TRANSLATION);
	}
	return;
};


void renderScene()
{
	renderer->clearBuffer(CLEAR_BUFFER_FLAGS::COLOR | CLEAR_BUFFER_FLAGS::DEPTH);
	for (auto m : scene)
	{
		renderer->submit(m);
	}
	renderer->frame();
	renderer->present();
}

int initialiseTestbench()
{
	std::string definePos = "#define POSITION " + std::to_string(POSITION) + "\n";
	std::string defineNor = "#define NORMAL " + std::to_string(NORMAL) + "\n";
	std::string defineUV = "#define TEXTCOORD " + std::to_string(TEXTCOORD) + "\n";
	std::string defineTX = "#define TRANSLATION " + std::to_string(TRANSLATION) + "\n";

	std::vector<std::vector<std::string>> materialDefs = {
		// vertex shader, fragment shader, defines
		// shader extension must be asked to the renderer
		// these strings should be constructed from the IA.h file!!!
		{ "VertexShader", "FragmentShader", definePos + defineNor + defineUV + defineTX },
		{ "VertexShader", "FragmentShader", definePos + defineNor + defineTX },
		{ "VertexShader", "FragmentShader", definePos + defineTX }
	};

	// triangle geometry:
	float4 triPos[3] = { { 0.0f,  0.5f, 0.0f, 1.0f },{ 0.5f, -0.5f, 0.0f, 1.0f },{ -0.5f, -0.5f, 0.0f, 1.0f } };
	float4 triNor[3] = { { 0.0f,  0.0f, 1.0f, 0.0f },{ 0.0f, 0.0f, 1.0f, 0.0f },{ 0.0f, 0.0f, 1.0f, 0.0f } };
	float2 triUV[3] =  { { 0.0f,  0.0f },{ 1.0f, 0.0f },{ 0.0f, 1.0f } };

	// load Materials.
	std::string shaderPath = renderer->getShaderPath();
	std::string shaderExtension = renderer->getShaderExtension();
	for (int i = 0; i < materialDefs.size(); i++)
	{
		// set material name from text file?
		Material* m = renderer->makeMaterial();
		m->setShader(shaderPath + materialDefs[i][0] + shaderExtension, Material::ShaderType::VS);
		m->setShader(shaderPath + materialDefs[i][1] + shaderExtension, Material::ShaderType::PS);

		m->addDefine(materialDefs[i][2], Material::ShaderType::VS);
		m->addDefine(materialDefs[i][2], Material::ShaderType::PS);

		std::string err;
		m->compileMaterial(err);
		
		materials.push_back(m);
	}

	// basic technique
	Technique* technique = new Technique();
	technique->material = materials[0];

	// Create a mesh array with 3 basic vertex buffers.
	for (int i = 0; i < 2; i++) {

		Mesh* m = renderer->makeMesh();

		constexpr auto numberOfElements = std::extent<decltype(triPos)>::value;

		VertexBuffer* pos = renderer->makeVertexBuffer();
		pos->setData(triPos, sizeof(triPos), VertexBuffer::STATIC);
		pos->bind(0, sizeof(triPos), POSITION);
		m->addIAVertexBufferBinding(pos, 0, numberOfElements, POSITION);

		VertexBuffer* nor = renderer->makeVertexBuffer();
		nor->setData(triNor, sizeof(triNor), VertexBuffer::STATIC);
		nor->bind(0, sizeof(triNor), NORMAL);
		m->addIAVertexBufferBinding(nor, 0, numberOfElements, NORMAL);

		VertexBuffer* uvs = renderer->makeVertexBuffer();
		uvs->setData(triUV, sizeof(triUV), VertexBuffer::STATIC);
		uvs->bind(0, sizeof(triUV), TEXTCOORD);
		m->addIAVertexBufferBinding(uvs, 0, numberOfElements, TEXTCOORD);

		m->txBuffer = renderer->makeConstantBuffer();

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
	renderer->setClearColor(0.5, 0.1, 0.1, 1.0);
	initialiseTestbench();
	run();
	finishApplication();
	return 0;
};
