#pragma once

#include <glew/glew.h>

#include <vector>

#include "Macros.h"

struct VertexAttribute
{
	VertexAttribute( unsigned int dataType, unsigned int count, unsigned char normalized );

	unsigned int DataType;
	unsigned int Count;
	unsigned char Normalized;

	static unsigned int GetSizeOfDataType( unsigned int dataType );
};

class VertexLayout
{
public:
	template<typename T>
	void Push( unsigned int count )
	{ 
		ASSERT( false );
	}

	template<>
	void Push<float>( unsigned int count )
	{
		mAttributes.emplace_back( GL_FLOAT, count, GL_FALSE );
		mStride += count * sizeof( float );
	}

	template<>
	void Push<unsigned int>( unsigned int count )
	{
		mAttributes.emplace_back( GL_UNSIGNED_INT, count, GL_FALSE );
		mStride += count * sizeof( unsigned int );
	}

	const std::vector<VertexAttribute> GetAttributes( ) const
	{
		return mAttributes;
	}

	unsigned int GetStride( ) const
	{
		return mStride;
	}

private:
	std::vector<VertexAttribute> mAttributes;
	unsigned int mStride = { 0 };
};

