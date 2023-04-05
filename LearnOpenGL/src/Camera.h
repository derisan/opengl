#pragma once

#include <glm/glm.hpp>


class Shader;


class Camera
{
public:
	void SetViewMatrix(const glm::vec3& position,
		const glm::vec3& target,
		const glm::vec3& up);

	void SetPerspectiveMatrix(float fov, float aspect, float nearZ, float farZ);
	void SetOrthoMatrix(float width, float height, float nearZ, float farZ);

	void SetCameraPos(const glm::vec3& position);
	void SetCameraFront(const glm::vec3& front);
	void SetCameraUp(const glm::vec3& up);
	void SetFOV(float fov);

	void Bind(const Shader& shader) const;

	glm::vec3 GetCameraPos() const;
	glm::vec3 GetCameraFront() const;
	glm::vec3 GetCameraUp() const;
	float GetFOV() const;

private:
	void bindViewMatrix(const Shader& shader) const;
	void bindProjectionMatrix(const Shader& shader) const;
	void updateViewMatrix();
	void updateProjectionMatrix(bool isPerspective = true);

private:
	// Matrices
	glm::mat4 mView = glm::mat4{ 1.0f };
	glm::mat4 mProjection = glm::mat4{ 1.0f };

	// View matrix attributes
	glm::vec3 mPosition = glm::vec3{ 0.0 };
	glm::vec3 mFront = glm::vec3{ 0.0 };
	glm::vec3 mUp = glm::vec3{ 0.0 };

	// Projection matrix attributes
	float mFOV = 0.0f;
	float mAspectRatio = 0.0f;
	float mNearZ = 0.0f;
	float mFarZ = 0.0f;
	float mWidth = 0.0f;
	float mHeight = 0.0f;
};

