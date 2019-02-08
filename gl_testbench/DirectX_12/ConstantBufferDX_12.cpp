#include "ConstantBufferDX_12.h"
#include "MaterialDX_12.h"

ConstantBufferDX_12::ConstantBufferDX_12(std::string NAME, unsigned int location, ID3D12Device* gDevice)
{
	name = NAME;
	this->location = location;

	D3D12_DESCRIPTOR_HEAP_DESC heapDescriptorDesc = {};
	heapDescriptorDesc.NumDescriptors = 1;
	heapDescriptorDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	heapDescriptorDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	gDevice->CreateDescriptorHeap(&heapDescriptorDesc, IID_PPV_ARGS(&gDescriptorHeap));

	// Not sure if this is correct
	// Trying to achieve 256-byte aligned CB
	UINT cbSizeAligned = (sizeof(ConstantBuffer) + 255) & ~255;

	D3D12_HEAP_PROPERTIES heapProperties = {};
	heapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;
	heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	heapProperties.CreationNodeMask = 1;
	heapProperties.VisibleNodeMask = 1;

	D3D12_RESOURCE_DESC resourceDesc = {};
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resourceDesc.Width = cbSizeAligned;
	resourceDesc.Height = 1;
	resourceDesc.DepthOrArraySize = 1;
	resourceDesc.MipLevels = 1;
	resourceDesc.SampleDesc.Count = 1;
	resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	gDevice->CreateCommittedResource(
		&heapProperties,
		D3D12_HEAP_FLAG_NONE,
		&resourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&gConstantBufferResource)
	);

	// Used for debugging
	gConstantBufferResource->SetName(L"CB Heap");

	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
	cbvDesc.BufferLocation = gConstantBufferResource->GetGPUVirtualAddress();
	cbvDesc.SizeInBytes = cbSizeAligned;
	gDevice->CreateConstantBufferView(&cbvDesc, gDescriptorHeap->GetCPUDescriptorHandleForHeapStart());
}

ConstantBufferDX_12::~ConstantBufferDX_12()
{
	/*SafeRelease(&gDescriptorHeap);
	SafeRelease(&gConstantBufferResource);*/
}

void ConstantBufferDX_12::setData(const void* data, size_t size, Material* m, unsigned int location)
{
	void* mappedMemory = nullptr;
	D3D12_RANGE readRange = { 0, 0 };

	if (SUCCEEDED(gConstantBufferResource->Map(0, &readRange, &mappedMemory)))
	{
		memcpy(mappedMemory, data, size);

		D3D12_RANGE writeRange = { 0, size };
		gConstantBufferResource->Unmap(0, &writeRange);
	}
}

void ConstantBufferDX_12::bind(Material* m)
{
	// CommandList SetDescriptorHeaps ?
}
