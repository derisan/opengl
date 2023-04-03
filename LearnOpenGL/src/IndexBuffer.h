#pragma once

#include <glew/glew.h>
#include <vector>
#include "GLObject.h"

class IndexBuffer
	: public GLObject
{
public:
	IndexBuffer( ) = default;
	IndexBuffer( const unsigned int* indices, unsigned int count );
	IndexBuffer( const std::vector<unsigned int>& indices );
	~IndexBuffer( );

	virtual void Bind( ) const override;
	virtual void Unbind( ) const override;

	unsigned int GetNumIndices( ) const;

private:
	void createBuffer( int bytes, const unsigned int* data );

private:
	unsigned int mNumIndices = { 0 };
};

