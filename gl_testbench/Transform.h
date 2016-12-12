#pragma once
class Transform
{
public:
	Transform();
	~Transform();
	virtual void translate(float x, float y, float z) = 0;
	virtual void rotate(float radians, float x, float y, float z) = 0;
};

