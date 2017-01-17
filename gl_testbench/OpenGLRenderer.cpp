#include <stdio.h>
#include "OpenGLRenderer.h"
#include <GL/glew.h>

#include "Material.h"
#include "MaterialGL.h"
#include "Mesh.h"
#include "MeshGL.h"
#include "Technique.h"
#include "ResourceBindingGL.h"
#include "RenderStateGL.h"
#include "VertexBufferGL.h"

OpenGLRenderer::OpenGLRenderer()
{
}

OpenGLRenderer::~OpenGLRenderer()
{
}

int OpenGLRenderer::shutdown()
{
	SDL_GL_DeleteContext(context);
	SDL_Quit();
	return 0;
}

Mesh* OpenGLRenderer::makeMesh() { 
	return new MeshGL(); 
}

std::string OpenGLRenderer::getShaderPath() {
	return std::string("..\\assets\\GL45\\");
}

VertexBuffer* OpenGLRenderer::makeVertexBuffer() { 
	return new VertexBufferGL(); 
};

Material* OpenGLRenderer::makeMaterial() { 
	return new MaterialGL(); 
}

ResourceBinding* OpenGLRenderer::makeResourceBinding() { 
	return new ResourceBindingGL(); 
}

RenderState* OpenGLRenderer::makeRenderState() { 
	return new RenderStateGL(); 
}

/* 
Technique* OpenGLRenderer::createTechnique()
{

}
*/

int OpenGLRenderer::initialize(unsigned int width, unsigned int height) {

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		fprintf(stderr, "%s", SDL_GetError());
		exit(-1);
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	window = SDL_CreateWindow("OpenGL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);
	context = SDL_GL_CreateContext(window);

	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		fprintf(stderr, "Error GLEW: %s\n", glewGetErrorString(err));
	}

	glViewport(0, 0, width, height);

	return 0;
}

/*
 Super simplified implementation of a work submission
 TODO.
*/
void OpenGLRenderer::submit(Mesh* mesh) 
{
	drawList.push_back(mesh);
};

/*
 Naive implementation, no re-ordering, checking for state changes, etc.
 TODO.
*/
void OpenGLRenderer::frame() 
{

	/*
	for (auto mesh : drawList)
	{
		// enable GLSL program
		mesh->technique->material->enable();

		// bind buffers for this mesh.
		// this implementation only has buffers in the Vertex Shader!
		// bind them all before drawing.
		size_t numberElements = 0;
		for (auto element : mesh->geometryBuffers) {
			mesh->bindIAVertexBuffer(element.first);
			numberElements = element.second.numElements;
		}

		// everything is bound!
		// always 0 because we are just generating gl_VertexId
		glDrawArrays(GL_POINTS, 0, numberElements);
	}
	drawList.clear();
	*/
};


void OpenGLRenderer::present()
{
	SDL_GL_SwapWindow(window);
};

void OpenGLRenderer::setClearColor(float r, float g, float b, float a)
{
	//clearColor[0] = r; clearColor[1] = g; clearColor[2] = b; clearColor[3] = a;
	glClearColor(r, g, b, a);
};

void OpenGLRenderer::clearBuffer(unsigned int flag) 
{
	glClear(GL_COLOR_BUFFER_BIT);
	return;
	// using is only valid inside the function!
	using namespace CLEAR_BUFFER_FLAGS;
	GLuint glFlags = BUFFER_MAP[flag & COLOR] | BUFFER_MAP[flag & DEPTH] | BUFFER_MAP[flag & STENCIL];
	glClear(glFlags);
};

void OpenGLRenderer::setRenderTarget(RenderTarget* rt) {};
void OpenGLRenderer::setRenderState(RenderState* ps) {};
