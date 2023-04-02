#pragma once

#include <glew/glew.h>
#include <string_view>

class Shader
{
public:
	Shader( std::string_view vsFilePath, std::string_view fsFilePath );
	~Shader( );

	void Bind( ) const;
	void Unbind( ) const;

private:
	std::string parseShader( std::string_view filePath );
	unsigned int createShader( std::string_view source, unsigned int shaderType );

private:
	unsigned int mProgramID = { 0 };
};

