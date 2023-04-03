#pragma once

#include "GLObject.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexLayout.h"

class VertexArray
	: public GLObject
{
public:
	VertexArray( );
	~VertexArray( );

	void SetVertexBuffer( const VertexBuffer& buffer, const VertexLayout& layout );
	void SetIndexBuffer( const IndexBuffer& buffer );

	virtual void Bind( ) const override;
	virtual void Unbind( ) const override;

	unsigned int GetNumIndices( ) const;

private:
	VertexBuffer mVertexBuffer;
	IndexBuffer mIndexBuffer;
};

