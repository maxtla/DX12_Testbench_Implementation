#pragma once

#include "../Sampler2D.h"

class DX_12Sampler2D :
	public Sampler2D
{
public:
	DX_12Sampler2D();
	~DX_12Sampler2D();
	void setMagFilter(FILTER filter);
	void setMinFilter(FILTER filter);
	void setWrap(WRAPPING s, WRAPPING t);

	//GLuint magFilter, minFilter, wrapS, wrapT;
	//GLuint samplerHandler = 0;
private:
};