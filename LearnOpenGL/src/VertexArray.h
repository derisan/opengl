#pragma once

#include "GLObject.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexLayout.h"

#include <unordered_map>
#include <memory>

class VertexArray
	: public GLObject
{
public:
	static std::shared_ptr<VertexArray> GetVertexArray( std::string_view vertexArrayName );
	static void Clear( );

private:
	inline static std::unordered_map<std::string, std::shared_ptr<VertexArray>> sVertexArrays;

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

