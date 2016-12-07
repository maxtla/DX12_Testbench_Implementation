#include <string>
#include "Renderer.h"

#include "Material.h"
#include "Technique.h"

#include <SDL_keyboard.h>
#include <SDL_events.h>

using namespace std;

Renderer* renderer;
Mesh* mTest;
void updateTestbench();

void run() {
	SDL_Event windowEvent;
	while (true)
	{
		if (SDL_PollEvent(&windowEvent))
		{
			if (windowEvent.type == SDL_QUIT) break;
			if (windowEvent.type == SDL_KEYUP && windowEvent.key.keysym.sym == SDLK_ESCAPE) break;
		}
		updateTestbench();
		// issue render calls
		// finalise rendering
		
		renderer->swapBuffers();
	}
	renderer->shutdown();
}

void updateTestbench()
{
	return;
};

int initialiseTestbench()
{
	mTest = new Mesh();

	MeshPart* part = new MeshPart();
	mTest->parts.push_back(part);
	part->technique = std::make_shared<Technique>();
	part->technique->passes.push_back(std::make_shared<Pass>());
	part->technique->passes.push_back(std::make_shared<Pass>());
	part->technique->passes.push_back(std::make_shared<Pass>());
	return 0;
}

void finishApplication() {
	delete mTest;
};

int main(int argc, char *argv[])
{
	renderer = Renderer::makeRenderer(Renderer::BACKEND::GL45);
	renderer->initialize();
	initialiseTestbench();
	run();
	finishApplication();
	return 0;
}