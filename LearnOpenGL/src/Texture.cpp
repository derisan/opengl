#include "Texture.h"

#include <glew/glew.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <filesystem>

#include "Macros.h"

std::string GetFileExtension( std::string_view fileName );

Texture::Texture( std::string_view fileName )
{
	loadTexture( fileName );
}

Texture::~Texture( )
{
	GLCall( glDeleteTextures( 1, &ObjectID ) );
}

void Texture::Bind( ) const
{
	GLCall( glBindTexture( GL_TEXTURE_2D, ObjectID ) );
}

void Texture::Bind( unsigned int slot ) const
{
	glActiveTexture( GL_TEXTURE0 + slot );
	Bind( );
}

void Texture::Unbind( ) const
{
	GLCall( glBindTexture( GL_TEXTURE_2D, 0 ) );
}

int Texture::GetWidth( ) const
{
	return mWidth;
}

int Texture::GetHeight( ) const
{
	return mHeight;
}

void Texture::loadTexture( std::string_view fileName )
{
	GLCall( glGenTextures( 1, &ObjectID ) );
	GLCall( glBindTexture( GL_TEXTURE_2D, ObjectID ) );

	GLCall( glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT ) );
	GLCall( glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT ) );
	GLCall( glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR ) );
	GLCall( glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR ) );

	stbi_set_flip_vertically_on_load( true );
	int channels = { 0 };
	auto textureData = stbi_load( fileName.data( ), &mWidth, &mHeight, &channels, 0 );
	auto extension = GetFileExtension( fileName );
	ASSERT( extension == "jpg" || extension == "png" );
	int format = extension == "jpg" ? GL_RGB : GL_RGBA;

	if ( textureData )
	{
		GLCall( glTexImage2D( GL_TEXTURE_2D, 0, format, mWidth, mHeight,
				0, format, GL_UNSIGNED_BYTE, textureData ) );
		GLCall( glGenerateMipmap( GL_TEXTURE_2D ) );
		stbi_image_free( textureData );
	}
	else
	{
		ASSERT( false );
	}
}

std::string GetFileExtension( std::string_view fileName )
{
	auto startIdx = fileName.rfind( "." );
	if ( startIdx == std::string_view::npos )
	{
		ASSERT( false );
	}

	return std::string{ fileName.substr( startIdx + 1 ).data( ) };
}