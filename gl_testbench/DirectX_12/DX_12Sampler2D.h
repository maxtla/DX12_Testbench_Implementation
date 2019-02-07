#pragma once

#include "../Sampler2D.h"
#include <d3d12.h>
#include "DX_12Renderer.h"

class DX_12Sampler2D :
	public Sampler2D
{
public:
	DX_12Sampler2D(DX_12Renderer* renderer);
	~DX_12Sampler2D();
	void setMagFilter(FILTER filter);
	void setMinFilter(FILTER filter);
	void setWrap(WRAPPING s, WRAPPING t);

	//GLuint magFilter, minFilter, wrapS, wrapT;
	//GLuint samplerHandler = 0;
	DX_12Renderer* _renderer = nullptr;
	ID3D12DescriptorHeap* _samplerHeap = nullptr;
private:
};