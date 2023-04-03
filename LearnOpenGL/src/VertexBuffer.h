#pragma once

#include <glew/glew.h>

#include "GLObject.h"

class VertexBuffer
	: public GLObject
{ 
public:
	VertexBuffer( ) = default;
	VertexBuffer( const void* vertices, int bytes );
	~VertexBuffer( );

	virtual void Bind( ) const override;
	virtual void Unbind( ) const override;
};

