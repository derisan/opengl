#pragma once

#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexLayout.h"

class VertexArray
{
public:
	VertexArray( );
	~VertexArray( );

	void SetVertexBuffer( const VertexBuffer& buffer, const VertexLayout& layout );
	void SetIndexBuffer( const IndexBuffer& buffer );

	void Bind( ) const;
	void Unbind( ) const;

	unsigned int GetNumIndices( ) const;

private:
	unsigned int mObjectID = { 0 };

	VertexBuffer mVertexBuffer;
	IndexBuffer mIndexBuffer;
};

