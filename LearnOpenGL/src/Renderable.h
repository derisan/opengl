#pragma once

#include <memory>
#include <vector>
#include <string>
#include <glm/glm.hpp>

class VertexArray;
class Texture;
class Shader;


class Renderable
{
	using TexturePtrVector = std::vector<std::shared_ptr<Texture>>;

public:
	virtual ~Renderable() = default;

	void SetVertexArray(const std::shared_ptr<VertexArray>& vertexArray);
	void SetTexture(const std::shared_ptr<Texture>& texture);
	void SetModelMatrix(const glm::mat4& mat);
	void SetColor(const glm::vec3& color);

	void Draw(const Shader& shader) const;

	glm::vec3 GetColor() const;

protected:
	virtual void setUniforms(const Shader& shader) const;

private:
	std::shared_ptr<VertexArray> mVertexArray;
	TexturePtrVector mTextures;
	glm::mat4 mModel = glm::mat4{ 1.0f };
	glm::vec3 mColor = glm::vec3{ 0.0f };
};

static std::string GetTextureUniformName(int slot);