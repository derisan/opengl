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
	void SetPosition(const glm::vec3& position);
	void SetRotation(const glm::vec3& rotation);
	void SetScale(const glm::vec3& scale);

	void Draw(const Shader& shader) const;

	glm::vec3 GetColor() const;
	glm::vec3 GetPosition() const;
	glm::vec3 GetRotation() const;
	glm::vec3 GetScale() const;

protected:
	virtual void setUniforms(const Shader& shader) const;

private:
	void updateModelMatrix();
	void updateITModelMatrix();

private:
	std::shared_ptr<VertexArray> mVertexArray;
	TexturePtrVector mTextures;
	glm::mat4 mModel{ 1.0f };
	glm::mat4 mInversedTransposedModel{ 1.0f };
	glm::vec3 mColor{ 0.0f };
	
	glm::vec3 mPosition{ 0.0f };
	glm::vec3 mRotation{ 0.0f };
	glm::vec3 mScale{ 1.0f };
};

static std::string GetTextureUniformName(int slot);