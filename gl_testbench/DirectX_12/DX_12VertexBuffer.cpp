#include "DX_12VertexBuffer.h"
#include <assert.h>

D3D12_HEAP_TYPE DX_12VertexBuffer::usageMapping[3] = { D3D12_HEAP_TYPE_UPLOAD, D3D12_HEAP_TYPE_READBACK, D3D12_HEAP_TYPE_DEFAULT };

DX_12VertexBuffer::DX_12VertexBuffer(DX_12Renderer* renderer, size_t size, VertexBuffer::DATA_USAGE usage)
{
	totalSize = size;
	
	D3D12_HEAP_PROPERTIES hp = {};
	hp.Type = usageMapping[usage];
	hp.CreationNodeMask = 1;
	hp.VisibleNodeMask = 1;

	D3D12_RESOURCE_DESC rd = {};
	rd.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	rd.Width = size;
	rd.Height = 1;
	rd.DepthOrArraySize = 1;
	rd.MipLevels = 1;
	rd.SampleDesc.Count = 1;
	rd.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//Creates both a resource and an implicit heap, such that the heap is big enough
	//to contain the entire resource and the resource is mapped to the heap.
	renderer->m_device->CreateCommittedResource(
		&hp,
		D3D12_HEAP_FLAG_NONE,
		&rd,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&_vertexBuffer));

	_vertexBuffer->SetName(L"vb heap");
}

DX_12VertexBuffer::~DX_12VertexBuffer()
{
	SafeRelease(&_vertexBuffer);
}

void DX_12VertexBuffer::setData(const void * data, size_t size, size_t offset)
{
	assert(size + offset <= totalSize);

	//Copy the triangle data to the vertex buffer.
	void* dataBegin = nullptr;
	D3D12_RANGE range = { 0, 0 }; //We do not intend to read this resource on the CPU.
	_vertexBuffer->Map(0, &range, &dataBegin);
	memcpy(((char*)dataBegin) + offset, data, sizeof(data));
	_vertexBuffer->Unmap(0, nullptr);

	//Initialize vertex buffer view, used in the render call.
	_vertexBuffer_View.BufferLocation = _vertexBuffer->GetGPUVirtualAddress();
	_vertexBuffer_View.StrideInBytes = size;
	_vertexBuffer_View.SizeInBytes = sizeof(data);

	unbind();
}

void DX_12VertexBuffer::bind(size_t offset, size_t size, unsigned int location)
{
	assert(offset + size <= totalSize);
}

void DX_12VertexBuffer::unbind()
{
}

size_t DX_12VertexBuffer::getSize()
{
	return totalSize;
}
