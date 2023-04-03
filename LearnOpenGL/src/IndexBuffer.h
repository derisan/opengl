#pragma once

#include <glew/glew.h>
#include "GLObject.h"

class IndexBuffer
	: public GLObject
{
public:
	IndexBuffer( ) = default;
	IndexBuffer( const unsigned int* indices, unsigned int count );
	~IndexBuffer( );

	virtual void Bind( ) const override;
	virtual void Unbind( ) const override;

	unsigned int GetNumIndices( ) const;

private:
	unsigned int mNumIndices = { 0 };
};

