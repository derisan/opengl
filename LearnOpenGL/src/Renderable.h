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
	void SetPosition(const glm::vec3& position);
	void SetRotation(const glm::vec3& rotation);
	void SetScale(const glm::vec3& scale);
	void SetAmbient(const glm::vec3& ambient);
	void SetDiffuse(const glm::vec3& diffuse);
	void SetSpecular(const glm::vec3& specular);
	void SetShininess(float shininess);

	void Draw(const Shader& shader) const;

	glm::vec3 GetPosition() const;
	glm::vec3 GetRotation() const;
	glm::vec3 GetScale() const;
	glm::vec3 GetAmbient() const;
	glm::vec3 GetDiffuse() const;
	glm::vec3 GetSpecular() const;
	float GetShininess() const;

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
	
	// Model stuffs
	glm::vec3 mPosition{ 0.0f };
	glm::vec3 mRotation{ 0.0f };
	glm::vec3 mScale{ 1.0f };

	// Color stuffs
	glm::vec3 mAmbient{ 0.0f };
	glm::vec3 mDiffuse{ 0.0f };
	glm::vec3 mSpecular{ 0.0f };
	float mShininess{ 32.0f };
};

static std::string GetTextureUniformName(int slot);