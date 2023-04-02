#pragma once

#include <glew/glew.h>

class VertexBuffer
{ 
public:
	VertexBuffer( ) = default;
	VertexBuffer( const void* vertices, int bytes );
	~VertexBuffer( );

	void Bind( ) const;
	void Unbind( ) const;

private:
	unsigned int mObjectID = { 0 };
};

