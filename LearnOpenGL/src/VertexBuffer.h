#pragma once

#include <glew/glew.h>
#include <vector>
#include "GLObject.h"

class VertexBuffer
	: public GLObject
{ 
public:
	VertexBuffer( ) = default;
	VertexBuffer( const void* vertices, int bytes );
	VertexBuffer( const std::vector<float>& vertices );
	~VertexBuffer( );

	virtual void Bind( ) const override;
	virtual void Unbind( ) const override;

private:
	void createBuffer( int bytes, const float* data );
};

