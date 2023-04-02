#pragma once

#include <glew/glew.h>

class IndexBuffer
{
public:
	IndexBuffer( ) = default;
	IndexBuffer( const unsigned int* indices, unsigned int count );
	~IndexBuffer( );

	void Bind( ) const;
	void Unbind( ) const;

	unsigned int GetNumIndices( ) const;

private:
	unsigned int mObjectID = { 0 };
	unsigned int mNumIndices = { 0 };
};

