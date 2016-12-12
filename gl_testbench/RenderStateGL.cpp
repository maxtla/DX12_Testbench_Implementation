#include <GL/glew.h>
#include "RenderStateGL.h"

RenderStateGL::RenderStateGL()
{
}

RenderStateGL::~RenderStateGL()
{
}

void RenderStateGL::setState(State& state)
{
	enableDepth(renderState.DEPTH);
	// and for the rest of the actions...
	
	// finally keep the new state in the struct
	this->renderState = state;
}

void RenderStateGL::enableDepth(bool value) {
	if (value)
		glEnable(GL_DEPTH);
	else
		glDisable(GL_DEPTH);
};

