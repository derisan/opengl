#include "VertexLayout.h"

VertexAttribute::VertexAttribute(unsigned int dataType,
	unsigned int count,
	unsigned char normalized)
	: DataType{ dataType }
	, Count{ count }
	, Normalized{ normalized }
{

}

unsigned int VertexAttribute::GetSizeOfDataType(unsigned int type)
{
	switch (type)
	{
	case GL_FLOAT:
	case GL_UNSIGNED_INT:
	case GL_INT:
		return 4;

	case GL_UNSIGNED_BYTE:
		return 1;

	default:
		ASSERT(false);
		return 0;
	}
}

const std::vector<VertexAttribute>& VertexLayout::GetAttributes() const
{
	return mAttributes;
}

unsigned int VertexLayout::GetStride() const
{
	return mStride;
}
