#include "VertexArray.h"

#include "Macros.h"

VertexArray::VertexArray( )
{
	GLCall( glCreateVertexArrays( 1, &mObjectID ) );
}

VertexArray::~VertexArray( )
{
	GLCall( glDeleteVertexArrays( 1, &mObjectID ) );
}

void VertexArray::SetVertexBuffer( const VertexBuffer& buffer, const VertexLayout& layout )
{
	mVertexBuffer = buffer;

	Bind( );
	mVertexBuffer.Bind( );

	const auto& attribs = layout.GetAttributes( );
	const auto stride = layout.GetStride( );
	unsigned int offset = 0;
	for ( auto i = 0; i < attribs.size( ); i++ )
	{
		const auto& attrib = attribs[ i ];
		GLCall( glEnableVertexAttribArray( i ) );
		GLCall( glVertexAttribPointer( i, attrib.Count, attrib.DataType, attrib.Normalized, stride,
									   reinterpret_cast< const void* >( offset ) ) );
		offset += attrib.Count * VertexAttribute::GetSizeOfDataType( attrib.DataType );
	}

	Unbind( );
	mVertexBuffer.Unbind( );
}

void VertexArray::SetIndexBuffer( const IndexBuffer& buffer )
{
	mIndexBuffer = buffer;

	Bind( );
	mIndexBuffer.Bind( );

	Unbind( );
	mIndexBuffer.Unbind( );
}

void VertexArray::Bind( ) const
{
	GLCall( glBindVertexArray( mObjectID ) );
}

void VertexArray::Unbind( ) const
{
	GLCall( glBindVertexArray( 0 ) );
}

unsigned int VertexArray::GetNumIndices( ) const
{
	return mIndexBuffer.GetNumIndices( );
}
