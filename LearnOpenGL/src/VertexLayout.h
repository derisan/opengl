#pragma once

#include <vector>
#include <glew/glew.h>
#include "Macros.h"


struct VertexAttribute
{
	VertexAttribute() = default;
	VertexAttribute(unsigned int dataType, unsigned int count, unsigned char normalized);

	unsigned int DataType = 0;
	unsigned int Count = 0;
	unsigned char Normalized = 0;

	static unsigned int GetSizeOfDataType(unsigned int dataType);
};

class VertexLayout
{
public:
	template<typename T>
	void Push(unsigned int count)
	{
		ASSERT(false);
	}

	template<>
	void Push<float>(unsigned int count)
	{
		mAttributes.emplace_back(GL_FLOAT, count, GL_FALSE);
		mStride += count * sizeof(float);
	}

	template<>
	void Push<unsigned int>(unsigned int count)
	{
		mAttributes.emplace_back(GL_UNSIGNED_INT, count, GL_FALSE);
		mStride += count * sizeof(unsigned int);
	}

	const std::vector<VertexAttribute>& GetAttributes() const;

	unsigned int GetStride() const;

private:
	std::vector<VertexAttribute> mAttributes;
	unsigned int mStride = 0;
};

