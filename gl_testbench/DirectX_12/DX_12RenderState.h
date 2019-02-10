#pragma once
#include "../RenderState.h"
#include "DX_12Renderer.h"

class DX_12RenderState : public RenderState
{
public:
	DX_12RenderState();
	~DX_12RenderState();
	void setWireFrame(bool);
	void set();
	const D3D12_RASTERIZER_DESC & GetRSDesc() { return m_rsDesc; }
private:
	bool m_wireFrame;
	D3D12_RASTERIZER_DESC m_rsDesc = {};
};

