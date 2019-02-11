#pragma once
#include <d3d12.h>
#include <minwindef.h>
#include "../ConstantBuffer.h"
#include "DX_12Helper.h"

class ConstantBufferDX_12 : public ConstantBuffer
{
public:
	ConstantBufferDX_12(std::string NAME, unsigned int location, ID3D12GraphicsCommandList * pCommandList);
	~ConstantBufferDX_12();
	void setData(const void* data, size_t size, Material* m, unsigned int location);
	void bind(Material*);

private:
	std::string name;
	UINT location;

	ID3D12GraphicsCommandList* m_commandList;
	float m_data[4];
};
