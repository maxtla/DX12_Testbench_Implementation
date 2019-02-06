#pragma once

#include "../Texture2D.h"
#include "DX_12Sampler2D.h"

#include <d3d12.h>
#include "DX_12Renderer.h"

class DX_12Texture2D :
	public Texture2D
{
public:
	DX_12Texture2D(DX_12Renderer* renderer);
	~DX_12Texture2D();

	int loadFromFile(std::string filename);
	void bind(unsigned int slot);

	// OPENGL HANDLE
	//GLuint textureHandle = 0;
	ID3D12Resource* _texture = nullptr;
	ID3D12DescriptorHeap* _srvHeap = nullptr;

	DX_12Renderer* _renderer = nullptr;
};