#include "Shader.h"

#include "Macros.h"

#include <fstream>
#include <sstream>

Shader::Shader( std::string_view vsFilePath, std::string_view fsFilePath )
{
	GLCall( mProgramID = glCreateProgram( ) );

	std::string vertexShaderSource = parseShader( vsFilePath );
	std::string fragmentShaderSource = parseShader( fsFilePath );

	auto vsID = createShader( vertexShaderSource, GL_VERTEX_SHADER );
	auto fsID = createShader( fragmentShaderSource, GL_FRAGMENT_SHADER );

	GLCall( glAttachShader( mProgramID, vsID ) );
	GLCall( glAttachShader( mProgramID, fsID ) );
	GLCall( glLinkProgram( mProgramID ) );

	GLCall( glDeleteShader( vsID ) );
	GLCall( glDeleteShader( fsID ) );
}

Shader::~Shader( )
{
	GLCall( glDeleteProgram( mProgramID ) );
}

void Shader::Bind( ) const
{
	GLCall( glUseProgram( mProgramID ) );
}

void Shader::Unbind( ) const
{
	GLCall( glUseProgram( 0 ) );
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
