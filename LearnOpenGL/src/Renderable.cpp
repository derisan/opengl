#include "Renderable.h"

#include "VertexArray.h"
#include "Texture.h"
#include "Shader.h"

#include <string>

std::string GetTextureUniformName( int slot );

void Renderable::SetVertexArray( const std::shared_ptr<VertexArray> vertexArray )
{
	mVertexArray = vertexArray;
}

void Renderable::SetTexture( const std::shared_ptr<Texture>& texture )
{
	mTextures.push_back( texture );
}

void Renderable::SetModelMatrix( const glm::mat4& mat )
{
	mModel = mat;
}

void Renderable::Draw( const Shader& shader ) const
{
	// Bind VAO
	mVertexArray->Bind( );

	// Bind textures
	for ( auto i = 0; i < mTextures.size( ); i++ )
	{
		mTextures[ i ]->Bind( i );
		shader.SetUniform1i( GetTextureUniformName( i ), i );
	}

	// Set model matrix uniform
	shader.SetUniformMat4( "uModel", mModel);

	// Draw call
	glDrawElements( GL_TRIANGLES, mVertexArray->GetNumIndices( ), GL_UNSIGNED_INT, nullptr );
}

std::string GetTextureUniformName( int slot )
{
	return "texture" + std::to_string( slot );
}

