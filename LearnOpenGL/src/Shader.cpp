#include "Shader.h"

#include "Macros.h"

#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <sstream>

Shader::Shader( std::string_view vsFilePath, std::string_view fsFilePath )
{
	GLCall( ObjectID = glCreateProgram( ) );

	std::string vertexShaderSource = parseShader( vsFilePath );
	std::string fragmentShaderSource = parseShader( fsFilePath );

	auto vsID = createShader( vertexShaderSource, GL_VERTEX_SHADER );
	auto fsID = createShader( fragmentShaderSource, GL_FRAGMENT_SHADER );

	GLCall( glAttachShader( ObjectID, vsID ) );
	GLCall( glAttachShader( ObjectID, fsID ) );
	GLCall( glLinkProgram( ObjectID ) );

	GLCall( glDeleteShader( vsID ) );
	GLCall( glDeleteShader( fsID ) );
}

Shader::~Shader( )
{
	GLCall( glDeleteProgram( ObjectID ) );
}

void Shader::Bind( ) const
{
	GLCall( glUseProgram( ObjectID ) );
}

void Shader::Unbind( ) const
{
	GLCall( glUseProgram( 0 ) );
}

void Shader::SetUniform4f( std::string_view uniformName, float x, float y, float z, float w ) const
{
	auto location = getUniformLocation( uniformName );
	Bind( );
	GLCall( glUniform4f( location, x, y, z, w ) );
}

void Shader::SetUniformMat4( std::string_view uniformName, const glm::mat4& mat ) const
{
	auto location = getUniformLocation( uniformName );
	Bind( );
	GLCall( glUniformMatrix4fv( location, 1, GL_FALSE, glm::value_ptr( mat ) ) );
}

void Shader::SetUniform1i( std::string_view uniformName, int x ) const
{
	auto location = getUniformLocation( uniformName );
	Bind( );
	GLCall( glUniform1i( location, x ) );
}

std::string Shader::parseShader( std::string_view filePath )
{
	std::ifstream file{ filePath.data( ) };
	ASSERT( file.is_open( ) );

	std::stringstream ss;
	ss << file.rdbuf( );

	return ss.str( );
}

unsigned int Shader::createShader( std::string_view source, unsigned int shaderType )
{
	GLCall( auto id = glCreateShader( shaderType ) );
	auto src = source.data( );
	GLCall( glShaderSource( id, 1, &src, nullptr ) );
	GLCall( glCompileShader( id ) );

	int result = { 0 };
	GLCall( glGetShaderiv( id, GL_COMPILE_STATUS, &result ) );
	if ( GL_FALSE == result )
	{
		int length = { 0 };
		GLCall( glGetShaderiv( id, GL_INFO_LOG_LENGTH, &length ) );
		char* message = reinterpret_cast< char* >( _malloca( length ) );
		GLCall( glGetShaderInfoLog( id, length, &length, message ) );
		std::cout << "Failed to compile " << ( shaderType == GL_VERTEX_SHADER ? "vertex shader" : "fragment shader" )
			<< std::endl;
		std::cout << message << std::endl;
		GLCall( glDeleteShader( id ) );
		ASSERT( false );
	}

	return id;
}

int Shader::getUniformLocation( std::string_view uniformName ) const
{
	auto iter = mUniformsMap.find( uniformName.data( ) );

	if ( iter != mUniformsMap.end( ) )
	{
		return iter->second;
	}
	else
	{
		int location = glGetUniformLocation( ObjectID, uniformName.data( ) );
		ASSERT( location != -1 );
		mUniformsMap.emplace( uniformName, location );
		return location;
	}
}
