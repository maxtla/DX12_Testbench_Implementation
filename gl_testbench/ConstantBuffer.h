#pragma once
class ConstantBuffer
{
public:
	ConstantBuffer();
	~ConstantBuffer();
	// set data will update the buffer associated, including whatever is necessary to
	// update the GPU memory.
	virtual void setData(const void* data, size_t size, unsigned int location) = 0;
};

//	enum DATA_USAGE { STATIC = 0, DYNAMIC = 1, DONTCARE = 2 };
