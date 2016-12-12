#pragma once
class RenderState
{
public:
	struct State {
		bool DEPTH;
	};

	RenderState();
	~RenderState();

	virtual void setState(State& state) = 0;
	State getState() { return renderState; }

	virtual void enableDepth(bool value = true) = 0;
protected:
	State renderState;
};

