#include "Renderable.h"
#include <string>
#include <glm/gtc/matrix_transform.hpp>
#include "VertexArray.h"
#include "Texture.h"
#include "Shader.h"
#include "Macros.h"

void Renderable::SetVertexArray(const std::shared_ptr<VertexArray>& vertexArray)
{
	mVertexArray = vertexArray;
}

void Renderable::SetTexture(const std::shared_ptr<Texture>& texture)
{
	mTextures.push_back(texture);
}

void Renderable::SetModelMatrix(const glm::mat4& mat)
{
	mModel = mat;
	updateITModelMatrix();
}

void Renderable::SetPosition(const glm::vec3& position)
{
	mPosition = position;
	updateModelMatrix();
}

void Renderable::SetRotation(const glm::vec3& rotation)
{
	mRotation = rotation;
	updateModelMatrix();
}

void Renderable::SetScale(const glm::vec3& scale)
{
	mScale = scale;
	updateModelMatrix();
}

void Renderable::SetAmbient(const glm::vec3& ambient)
{
	mAmbient = ambient;
}

void Renderable::SetDiffuse(const glm::vec3& diffuse)
{
	mDiffuse = diffuse;
}

void Renderable::SetSpecular(const glm::vec3& specular)
{
	mSpecular = specular;
}

void Renderable::SetShininess(float shininess)
{
	mShininess = shininess;
}

void Renderable::Draw(const Shader& shader) const
{
	mVertexArray->Bind();
	setUniforms(shader);
	glDrawElements(GL_TRIANGLES, mVertexArray->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
}

glm::vec3 Renderable::GetPosition() const
{
	return mPosition;
}

glm::vec3 Renderable::GetRotation() const
{
	return mRotation;
}

glm::vec3 Renderable::GetScale() const
{
	return mScale;
}

glm::vec3 Renderable::GetAmbient() const
{
	return mAmbient;
}

glm::vec3 Renderable::GetDiffuse() const
{
	return mDiffuse;
}

glm::vec3 Renderable::GetSpecular() const
{
	return mSpecular;
}

float Renderable::GetShininess() const
{
	return mShininess;
}

void Renderable::setUniforms(const Shader& shader) const
{
	for (auto i = 0; i < mTextures.size(); i++)
	{
		mTextures[i]->Bind(i);
		shader.SetUniform1i(GetTextureUniformName(i), i);
	}
	shader.SetUniformMat4("uModel", mModel);
	shader.SetUniformMat4("uITModel", mInversedTransposedModel);
}

void Renderable::updateModelMatrix()
{
	mModel = glm::mat4{ 1.0f };
	mModel = glm::translate(mModel, mPosition);
	mModel = glm::rotate(mModel, glm::radians(mRotation.x), glm::vec3{ 1.0f, 0.0f, 0.0f });
	mModel = glm::rotate(mModel, glm::radians(mRotation.y), glm::vec3{ 0.0f, 1.0f, 0.0f });
	mModel = glm::rotate(mModel, glm::radians(mRotation.z), glm::vec3{ 0.0f, 0.0f, 1.0f });
	mModel = glm::scale(mModel, mScale);
	updateITModelMatrix();
}

void Renderable::updateITModelMatrix()
{
	mInversedTransposedModel = glm::transpose(glm::inverse(mModel));
}

std::string GetTextureUniformName(int slot)
{
	switch (slot)
	{
	case 0:
		return "uMaterial.DiffuseMap";
	case 1:
		return "uMaterial.SpecularMap";
	default:
		ASSERT(false);
		return "";
	}
}

