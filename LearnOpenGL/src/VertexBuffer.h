#pragma once

#include <vector>
#include <glew/glew.h>
#include "GLObject.h"


class VertexBuffer
	: public GLObject
{
private:
	inline static const unsigned int kVertexBufferType = GL_ARRAY_BUFFER;

public:
	VertexBuffer() = default;
	VertexBuffer(const float* vertices, int bytes);
	VertexBuffer(const std::vector<float>& vertices);
	~VertexBuffer();

	virtual void Bind() const override;
	virtual void Unbind() const override;

private:
	void createBuffer(const float* data, int bytes);
};

