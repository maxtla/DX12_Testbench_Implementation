#include <string>
#include "Renderer.h"

#include "Material.h";

#include <SDL_keyboard.h>
#include <SDL_events.h>

using namespace std;

Renderer* renderer;
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
	return 0;
}

int main(int argc, char *argv[])
{
	renderer = Renderer::makeRenderer(Renderer::BACKEND::GL45);
	renderer->initialize();
	initialiseTestbench();
	run();
	return 0;
}