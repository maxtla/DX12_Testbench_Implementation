#include "DX_12Technique.h"



DX_12Technique::DX_12Technique(Material * m, RenderState * rs, DX_12Renderer * pRenderer) : Technique(m, rs)
{
	MaterialDX_12 * pDX_m = dynamic_cast<MaterialDX_12*>(m);
	DX_12RenderState * pDX_rs = dynamic_cast<DX_12RenderState*>(rs);

	assert(pDX_m);
	assert(pDX_rs);

	auto shaderCodes = pDX_m->GetShaderCodePtrs();
	auto rsDesc = pDX_rs->GetRSDesc();

	// Define the vertex input layout. BASED ON IA.h macros and Number of Vertex buffers for correct input slot
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 2, DXGI_FORMAT_R32G32_FLOAT, 2, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	};

	//Define the blend state as default
	D3D12_BLEND_DESC blendDesc = {};
	blendDesc.AlphaToCoverageEnable = FALSE;
	blendDesc.IndependentBlendEnable = FALSE;
	const D3D12_RENDER_TARGET_BLEND_DESC defaultRenderTargetBlendDesc =
	{
		FALSE,FALSE,
		D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
		D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
		D3D12_LOGIC_OP_NOOP,
		D3D12_COLOR_WRITE_ENABLE_ALL,
	};
	for (UINT i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i)
		blendDesc.RenderTarget[i] = defaultRenderTargetBlendDesc;

	// Describe and create the graphics pipeline state object (PSO).
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
	psoDesc.InputLayout = { inputElementDescs, _ARRAYSIZE(inputElementDescs) };
	psoDesc.pRootSignature = pRenderer->m_rootSignature;
	psoDesc.VS = { reinterpret_cast<UINT8*>(shaderCodes.first->GetBufferPointer()), shaderCodes.first->GetBufferSize() };
	psoDesc.PS = { reinterpret_cast<UINT8*>(shaderCodes.second->GetBufferPointer()), shaderCodes.second->GetBufferSize() };
	psoDesc.RasterizerState = rsDesc;
	psoDesc.BlendState = blendDesc;
	psoDesc.DepthStencilState.DepthEnable = FALSE;
	psoDesc.DepthStencilState.StencilEnable = FALSE;
	psoDesc.SampleMask = UINT_MAX;
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	psoDesc.NumRenderTargets = pRenderer->FRAME_COUNT;
	psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	psoDesc.SampleDesc.Count = 1;

	ThrowIfFailed(pRenderer->m_device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_PSO)));
}


DX_12Technique::~DX_12Technique()
{
}
