#pragma once

#include <glew/glew.h>
#include <glm/glm.hpp>
#include <string_view>
#include <unordered_map>

#include "GLObject.h"

class Shader
	: public GLObject
{
public:
	Shader( std::string_view vsFilePath, std::string_view fsFilePath );
	~Shader( );

	virtual void Bind( ) const override;
	virtual void Unbind( ) const override;

	void SetUniform4f( std::string_view uniformName, float x, float y, float z, float w );
	void SetUniformMat4( std::string_view uniformName, const glm::mat4& mat );
	void SetUniform1i( std::string_view uniformName, int x );

private:
	std::string parseShader( std::string_view filePath );
	unsigned int createShader( std::string_view source, unsigned int shaderType );

	int getUniformLocation( std::string_view uniformName );

private:
	std::unordered_map<std::string, int> mUniformsMap;
};

