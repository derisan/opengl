#include "IndexBuffer.h"

#include "Macros.h"

const unsigned int kIndexBufferType = GL_ELEMENT_ARRAY_BUFFER;

IndexBuffer::IndexBuffer( const unsigned int* indices, unsigned int count )
	: mNumIndices{ count }
{
	GLCall(glGenBuffers( 1, &mObjectID ));
	Bind( );
	GLCall(glBufferData( kIndexBufferType, sizeof( unsigned int ) * count, 
				  reinterpret_cast< const void* >( indices ), GL_STATIC_DRAW ));
}

IndexBuffer::~IndexBuffer( )
{
	GLCall(glDeleteBuffers( 1, &mObjectID ));
}

void IndexBuffer::Bind( ) const
{
	GLCall(glBindBuffer( kIndexBufferType, mObjectID ));
}

void IndexBuffer::Unbind( ) const
{
	GLCall(glBindBuffer( kIndexBufferType, 0 ));
}

unsigned int IndexBuffer::GetNumIndices( ) const
{
	return mNumIndices;
}
