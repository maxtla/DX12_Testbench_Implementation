#pragma once

#include "Renderer.h"

#include <SDL.h>
//#include <SDL_opengl.h>

#pragma comment(lib,"glew32.lib")
#pragma comment(lib,"SDL2.lib")
#pragma comment(lib,"SDL2main.lib")

class OpenGLRenderer : public Renderer
{
public:
	OpenGLRenderer();
	~OpenGLRenderer();

	void setClearColor(float, float, float, float);
	void clearBuffer(CLEAR_BUFFER_FLAGS);
	void setRenderTarget(RenderTarget* rt); // complete parameters
	void setRenderState(RenderState* ps);
	void draw(Mesh* mesh, DrawInfo* data = nullptr);

	void swapBuffers();
	int initialize(unsigned int width = 640, unsigned int height = 480);
	int shutdown();

private:
	SDL_Window* window;
	SDL_GLContext context;
	int initializeOpenGL(int major, int minor, unsigned int width, unsigned int height);

};

