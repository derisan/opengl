#include "VertexBuffer.h"

#include "Macros.h"

const unsigned int kVertexBufferType = GL_ARRAY_BUFFER;

VertexBuffer::VertexBuffer( const void* vertices, int bytes )
{
	GLCall( glGenBuffers( 1, &mObjectID ) );
	Bind( );
	GLCall( glBufferData( kVertexBufferType, bytes, vertices, GL_STATIC_DRAW ) );
}

VertexBuffer::~VertexBuffer( )
{
	GLCall( glDeleteBuffers( 1, &mObjectID ) );
}

void VertexBuffer::Bind( ) const
{
	GLCall( glBindBuffer( kVertexBufferType, mObjectID ) );
}

void VertexBuffer::Unbind( ) const
{
	GLCall( glBindBuffer( kVertexBufferType, 0 ) );
}
