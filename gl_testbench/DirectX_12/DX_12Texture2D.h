#pragma once

#include "../Texture2D.h"
#include "DX_12Sampler2D.h"


class DX_12Texture2D :
	public Texture2D
{
public:
	DX_12Texture2D();
	~DX_12Texture2D();

	int loadFromFile(std::string filename);
	void bind(unsigned int slot);

	// OPENGL HANDLE
	//GLuint textureHandle = 0;
};