#include "VertexArray.h"

#include "Macros.h"

std::shared_ptr<VertexArray> VertexArray::GetVertexArray( std::string_view vertexArrayName )
{
	auto iter = sVertexArrays.find( vertexArrayName.data( ) );

	if ( iter != sVertexArrays.end( ) )
	{
		return iter->second;
	}

	// (TODO): create VBO, layout, EBO and VAO

	std::vector<float> vertices;
	std::vector<unsigned int> indices;

	if ( vertexArrayName == "Rectangle" )
	{
		vertices.reserve( 20 );
		indices.reserve( 6 );

		vertices.assign( {
			-0.5f, -0.5f,  0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f,  0.0f, 1.0f, 1.0f,
			 -0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
						 } );

		indices.assign( {
			0, 1, 2,
			0, 2, 3
						} );
	}
	else if ( vertexArrayName == "Cube" )
	{
		vertices.reserve( 40 );
		indices.reserve( 36 );

		vertices.assign( {
			-0.5f, -0.5f,  0.5f, 0.0f, 0.0f,//0
			 0.5f, -0.5f,  0.5f, 1.0f, 0.0f,//1
			-0.5f,  0.5f,  0.5f, 0.0f, 1.0f,//2
			 0.5f,  0.5f,  0.5f, 1.0f, 1.0f,//3
			-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,//4
			 0.5f, -0.5f, -0.5f, 1.0f, 0.0f,//5
			-0.5f,  0.5f, -0.5f, 0.0f, 1.0f,//6
			 0.5f,  0.5f, -0.5f, 1.0f, 1.0f,//7
						 } );

		indices.assign( {
			//Top
			2, 6, 7,
			2, 3, 7,

			//Bottom
			0, 4, 5,
			0, 1, 5,

			//Left
			0, 2, 6,
			0, 4, 6,

			//Right
			1, 3, 7,
			1, 5, 7,

			//Front
			0, 2, 3,
			0, 1, 3,

			//Back
			4, 6, 7,
			4, 5, 7
						} );
	}

	VertexBuffer vb{ vertices };
	VertexLayout layout;
	layout.Push<float>( 3 );
	layout.Push<float>( 2 );
	IndexBuffer ib{ indices };

	auto vertexArray = std::make_shared<VertexArray>( );
	vertexArray->SetVertexBuffer( vb, layout );
	vertexArray->SetIndexBuffer( ib );

	sVertexArrays.emplace( vertexArrayName.data(), vertexArray );
	return vertexArray;
}

void VertexArray::Clear( )
{
	sVertexArrays.clear( );
}

VertexArray::VertexArray( )
{
	GLCall( glCreateVertexArrays( 1, &ObjectID ) );
}

VertexArray::~VertexArray( )
{
	GLCall( glDeleteVertexArrays( 1, &ObjectID ) );
}

void VertexArray::SetVertexBuffer( const VertexBuffer& buffer, const VertexLayout& layout )
{
	mVertexBuffer = buffer;

	Bind( );
	mVertexBuffer.Bind( );

	const auto& attribs = layout.GetAttributes( );
	const auto stride = layout.GetStride( );
	unsigned long long offset = 0;
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
	GLCall( glBindVertexArray( ObjectID ) );
}

void VertexArray::Unbind( ) const
{
	GLCall( glBindVertexArray( 0 ) );
}

unsigned int VertexArray::GetNumIndices( ) const
{
	return mIndexBuffer.GetNumIndices( );
}
