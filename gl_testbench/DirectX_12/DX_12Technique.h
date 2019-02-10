#pragma once
#include "../Technique.h"
#include "DX_12Renderer.h"
#include "MaterialDX_12.h"
#include "DX_12RenderState.h"

class DX_12Technique : public Technique
{
public:
	DX_12Technique(Material * m, RenderState * rs, DX_12Renderer * pRenderer);
	~DX_12Technique();

	ID3D12PipelineState * m_PSO = NULL;
};

