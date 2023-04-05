#pragma once

#include <vector>
#include <glew/glew.h>
#include "GLObject.h"


class IndexBuffer
	: public GLObject
{
private:
	inline static const unsigned int kIndexBufferType = GL_ELEMENT_ARRAY_BUFFER;

public:
	IndexBuffer() = default;
	IndexBuffer(const unsigned int* indices, unsigned int count);
	IndexBuffer(const std::vector<unsigned int>& indices);
	~IndexBuffer();

	virtual void Bind() const override;
	virtual void Unbind() const override;

	unsigned int GetNumIndices() const;

private:
	void createBuffer(int bytes, const unsigned int* data);

private:
	unsigned int mNumIndices = 0;
};

