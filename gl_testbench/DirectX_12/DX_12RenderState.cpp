#include "DX_12RenderState.h"



DX_12RenderState::DX_12RenderState()
{
	m_rsDesc.AntialiasedLineEnable = TRUE;
	m_rsDesc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
	m_rsDesc.CullMode = D3D12_CULL_MODE_NONE;
	m_rsDesc.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
	m_rsDesc.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
	m_rsDesc.DepthClipEnable = FALSE;
	m_rsDesc.ForcedSampleCount = 0;
	m_rsDesc.FrontCounterClockwise = FALSE;
	m_rsDesc.MultisampleEnable = FALSE;
	m_rsDesc.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
	m_rsDesc.FillMode = D3D12_FILL_MODE_SOLID;
}


DX_12RenderState::~DX_12RenderState()
{
}

void DX_12RenderState::setWireFrame(bool b)
{
	m_wireFrame = b;
	if (b)
		m_rsDesc.FillMode = D3D12_FILL_MODE_WIREFRAME;
	else
		m_rsDesc.FillMode = D3D12_FILL_MODE_SOLID;
}

void DX_12RenderState::set()
{
	//This seems very unnecessary

	m_rsDesc.AntialiasedLineEnable = TRUE;
	m_rsDesc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
	m_rsDesc.CullMode = D3D12_CULL_MODE_NONE;
	m_rsDesc.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
	m_rsDesc.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
	m_rsDesc.DepthClipEnable = FALSE;
	m_rsDesc.ForcedSampleCount = 0;
	m_rsDesc.FrontCounterClockwise = FALSE;
	m_rsDesc.MultisampleEnable = FALSE;
	m_rsDesc.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;

	if (m_wireFrame)
		m_rsDesc.FillMode = D3D12_FILL_MODE_WIREFRAME;
	else
		m_rsDesc.FillMode = D3D12_FILL_MODE_SOLID;
}
