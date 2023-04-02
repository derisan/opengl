#pragma once

#include <glew/glew.h>
#include <string_view>
#include <unordered_map>

class Shader
{
public:
	Shader( std::string_view vsFilePath, std::string_view fsFilePath );
	~Shader( );

	void Bind( ) const;
	void Unbind( ) const;

	void SetUniform4f( std::string_view uniformName, float x, float y, float z, float w );

private:
	std::string parseShader( std::string_view filePath );
	unsigned int createShader( std::string_view source, unsigned int shaderType );

	int getUniformLocation( std::string_view uniformName );

private:
	unsigned int mProgramID = { 0 };
	std::unordered_map<std::string, int> mUniformsMap;
};

