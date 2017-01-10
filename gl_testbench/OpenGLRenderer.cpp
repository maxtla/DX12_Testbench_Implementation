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

void OpenGLRenderer::swapBuffers()
{
	SDL_GL_SwapWindow(window);
}

Mesh* OpenGLRenderer::makeMesh()
{
	// return an OpenGL material
	MeshGL* m = new MeshGL();
	return m;
}


Material* OpenGLRenderer::makeMaterial()
{
	// return an OpenGL material
	MaterialGL* m = new MaterialGL();
	return m;
}

ResourceBinding* OpenGLRenderer::makeResourceBinding()
{
	ResourceBindingGL* m = new ResourceBindingGL();
	return m;
}

RenderState* OpenGLRenderer::makeRenderState() {
	RenderStateGL* rs = new RenderStateGL();
	return rs;
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

	/*
	MaterialGL test0;
	test0.setShader("..\\assets\\GL45\\VertexShader.glsl", Material::ShaderType::VS);
	test0.setShader("..\\assets\\GL45\\Fragment.glsl", Material::ShaderType::PS);
	std::string errString;
	int res = test0.compileMaterial(errString);
	*/


	return 0;
}


void OpenGLRenderer::setClearColor(float, float, float, float) {};
void OpenGLRenderer::clearBuffer(unsigned int) {};
void OpenGLRenderer::setRenderTarget(RenderTarget* rt) {};
void OpenGLRenderer::setRenderState(RenderState* ps) {};
void OpenGLRenderer::draw(Mesh* mesh, DrawInfo* data) {};