#pragma once

#include <d3d12.h>
#include "../VertexBuffer.h"
#include "DX_12Renderer.h"

class DX_12VertexBuffer :
	public VertexBuffer
{
public:

	DX_12VertexBuffer(DX_12Renderer* renderer, size_t size, VertexBuffer::DATA_USAGE usage);
	~DX_12VertexBuffer();

	void setData(const void* data, size_t size, size_t offset);
	void bind(size_t offset, size_t size, unsigned int location);
	void unbind();
	size_t getSize();

	static D3D12_HEAP_TYPE usageMapping[3];

	D3D12_VERTEX_BUFFER_VIEW& GetVertexBufferView() { return _vertexBuffer_View; };
private:
	size_t totalSize;
	ID3D12Resource* _vertexBuffer = nullptr;
	D3D12_VERTEX_BUFFER_VIEW _vertexBuffer_View;
};