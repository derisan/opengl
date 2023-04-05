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
	void SetVertexArray(const std::shared_ptr<VertexArray>& vertexArray);
	void SetTexture(const std::shared_ptr<Texture>& texture);
	void SetModelMatrix(const glm::mat4& mat);

	void Draw(const Shader& shader) const;

private:
	std::shared_ptr<VertexArray> mVertexArray;
	TexturePtrVector mTextures;
	glm::mat4 mModel = glm::mat4{ 1.0f };
};

static std::string GetTextureUniformName(int slot);