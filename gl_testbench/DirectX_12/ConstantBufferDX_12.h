#pragma once
#include <d3d12.h>
#include <minwindef.h>
#include "../ConstantBuffer.h"

class ConstantBufferDX_12 : public ConstantBuffer
{
public:
	ConstantBufferDX_12(std::string NAME, unsigned int location, ID3D12Device* gDevice);
	~ConstantBufferDX_12();
	void setData(const void* data, size_t size, Material* m, unsigned int location);
	void bind(Material*);

private:
	std::string name;
	UINT location;

	ID3D12DescriptorHeap* gDescriptorHeap = {};
	ID3D12Resource1* gConstantBufferResource = {};
};

template <class T> inline void SafeRelease(T **ppT)
{
	if (*ppT)
	{
		(*ppT)->Release();
		*ppT = NULL;
	}
}