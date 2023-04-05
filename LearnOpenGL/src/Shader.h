#pragma once

#include <string_view>
#include <unordered_map>
#include <glew/glew.h>
#include <glm/glm.hpp>
#include "GLObject.h"


class Shader
	: public GLObject
{
	using UniformMap = std::unordered_map<std::string, int>;

public:
	Shader(std::string_view vsFilePath, std::string_view fsFilePath);
	~Shader();

	virtual void Bind() const override;
	virtual void Unbind() const override;

	void SetUniform4f(std::string_view uniformName, float x, float y, float z, float w) const;
	void SetUniformMat4(std::string_view uniformName, const glm::mat4& mat) const;
	void SetUniform1i(std::string_view uniformName, int x) const;
	void SetUniform3f(std::string_view uniformName, float x, float y, float z) const;
	void SetUniform3f(std::string_view uniformName, const glm::vec3& vec) const;

private:
	std::string parseShader(std::string_view filePath) const;
	unsigned int createShader(std::string_view source, unsigned int shaderType) const;
	int getUniformLocation(std::string_view uniformName) const;

private:
	mutable UniformMap mUniformMap;
};

