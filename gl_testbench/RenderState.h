#pragma once
class RenderState
{
public:
	RenderState();
	~RenderState();

	virtual void setDepthTest(bool) = 0;
	virtual void setDepthWrite(bool) = 0;
	virtual void setWireFrame(bool) = 0;
};

