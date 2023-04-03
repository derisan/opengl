#include "VertexBuffer.h"

#include "Macros.h"

const unsigned int kVertexBufferType = GL_ARRAY_BUFFER;

VertexBuffer::VertexBuffer( const void* vertices, int bytes )
{
	createBuffer( bytes, static_cast< const float* >( vertices ) );
}

VertexBuffer::VertexBuffer( const std::vector<float>& vertices )
{
	int bytes = static_cast< int >( sizeof( float ) * vertices.size( ) );
	createBuffer( bytes, vertices.data( ) );
}

VertexBuffer::~VertexBuffer( )
{
	GLCall( glDeleteBuffers( 1, &ObjectID ) );
}

void VertexBuffer::Bind( ) const
{
	GLCall( glBindBuffer( kVertexBufferType, ObjectID ) );
}

void VertexBuffer::Unbind( ) const
{
	GLCall( glBindBuffer( kVertexBufferType, 0 ) );
}

void VertexBuffer::createBuffer( int bytes, const float* data )
{
	GLCall( glGenBuffers( 1, &ObjectID ) );
	Bind( );
	GLCall( glBufferData( kVertexBufferType, bytes, data, GL_STATIC_DRAW ) );
}
