#include "DX_12Sampler2D.h"


DX_12Sampler2D::DX_12Sampler2D(DX_12Renderer* renderer) : _renderer(renderer)
{
	// create sampler descriptor heap
	D3D12_DESCRIPTOR_HEAP_DESC descHeapSampler = {};
	descHeapSampler.NumDescriptors = 1;
	descHeapSampler.Type = D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER;
	descHeapSampler.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	_renderer->m_device->CreateDescriptorHeap(&descHeapSampler, IID_PPV_ARGS(&_samplerHeap));

	// create sampler descriptor in the sample descriptor heap
	D3D12_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(D3D12_SAMPLER_DESC));
	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	_renderer->m_device->CreateSampler(&samplerDesc, _samplerHeap->GetCPUDescriptorHandleForHeapStart());
}

DX_12Sampler2D::~DX_12Sampler2D()
{
	SafeRelease(&_samplerHeap);
}

void DX_12Sampler2D::setMagFilter(FILTER filter)
{
}

void DX_12Sampler2D::setMinFilter(FILTER filter)
{
}

void DX_12Sampler2D::setWrap(WRAPPING s, WRAPPING t)
{
}
