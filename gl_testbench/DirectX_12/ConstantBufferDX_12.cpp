#include "ConstantBufferDX_12.h"
#include "MaterialDX_12.h"

ConstantBufferDX_12::ConstantBufferDX_12(std::string NAME, unsigned int location, ID3D12GraphicsCommandList * pCommandList) : m_commandList(pCommandList)
{
	name = NAME;
	this->location = location;
}

ConstantBufferDX_12::~ConstantBufferDX_12()
{

}

void ConstantBufferDX_12::setData(const void* data, size_t size, Material* m, unsigned int location)
{
	float* dataArr = (float*)data;
	for (int i = 0; i < 4; i++)
		m_data[i] = dataArr[i];
}

void ConstantBufferDX_12::bind(Material* m)
{
	m_commandList->SetGraphicsRoot32BitConstants(0, 4, (const void*)m_data, 0);
}
