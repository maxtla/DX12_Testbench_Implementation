#include <string>
#include <SDL_keyboard.h>
#include <SDL_events.h>


//#include "Material.h"
//#include "Technique.h"
#include "Renderer.h"
#include "Mesh.h"


using namespace std;
Renderer* renderer;

// flat scene
vector<Mesh*> scene;
vector<Material*> materials;

void updateScene();
void renderScene();

void run() {
	SDL_Event windowEvent;
	while (true)
	{
		if (SDL_PollEvent(&windowEvent))
		{
			if (windowEvent.type == SDL_QUIT) break;
			if (windowEvent.type == SDL_KEYUP && windowEvent.key.keysym.sym == SDLK_ESCAPE) break;
		}
		renderer->clearBuffer(Renderer::CLEAR_BUFFER_FLAGS::COLOR | Renderer::CLEAR_BUFFER_FLAGS::DEPTH);
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
	for (auto m : scene)
	{
		renderer->draw(m);
	}
}

int initialiseTestbench()
{
	// load meshes from model file.
	// load TriangleMesh from static method.
	// 

	// load Materials.
	for (int i = 0; i < 10; i++)
	{
		// set material name from text file?
		Material* m = renderer->makeMaterial();
		materials.push_back(m);
	}

	// Create a mesh array, to push API interaction
	// each mesh will have only ONE meshPart.
	for (int i = 0; i < 50000; i++) {
		Mesh* m = new Mesh();
		//m->setMaterial(materials[i%materials.size()]);
		scene.push_back(m);
	}

	//MeshPart* part = new MeshPart();
	//mTest->parts.push_back(part);
	//part->technique = new Technique();
	//part->technique->passes.push_back(std::make_shared<Pass>());
	//part->technique->passes.push_back(std::make_shared<Pass>());
	//part->technique->passes.push_back(std::make_shared<Pass>());
	return 0;
}

void finishApplication() {
};

int main(int argc, char *argv[])
{
	renderer = Renderer::makeRenderer(Renderer::BACKEND::GL45);
	renderer->initialize();
	initialiseTestbench();
	run();
	finishApplication();
	return 0;
};
