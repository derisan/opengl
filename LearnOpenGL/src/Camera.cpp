#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include "Shader.h"

void Camera::SetPerspectiveMatrix(float fov, float aspect, float nearZ, float farZ)
{
	mFOV = fov;
	mAspectRatio = aspect;
	mNearZ = nearZ;
	mFarZ = farZ;
	updateProjectionMatrix();
}

void Camera::SetOrthoMatrix(float width, float height, float nearZ, float farZ)
{
	mWidth = width;
	mHeight = height;
	mNearZ = nearZ;
	mFarZ = farZ;
	updateProjectionMatrix(false);
}

void Camera::SetViewMatrix(const glm::vec3& position, const glm::vec3& target, const glm::vec3& up)
{
	SetCameraPos(position);
	SetCameraFront(target);
	SetCameraUp(up);
	updateViewMatrix();
}

void Camera::SetCameraPos(const glm::vec3& position)
{
	mPosition = position;
	updateViewMatrix();
}

void Camera::SetCameraFront(const glm::vec3& front)
{
	mFront = glm::normalize(front);
	updateViewMatrix();
}

void Camera::SetCameraUp(const glm::vec3& up)
{
	mUp = glm::normalize(up);
	updateViewMatrix();
}

void Camera::SetFOV(float fov)
{
	mFOV = fov;
	updateProjectionMatrix();
}

void Camera::Bind(const Shader& shader) const
{
	bindViewMatrix(shader);
	bindProjectionMatrix(shader);
}

glm::vec3 Camera::GetCameraPos() const
{
	return mPosition;
}

glm::vec3 Camera::GetCameraFront() const
{
	return mFront;
}

glm::vec3 Camera::GetCameraUp() const
{
	return mUp;
}

float Camera::GetFOV() const
{
	return mFOV;
}

void Camera::bindViewMatrix(const Shader& shader) const
{
	shader.SetUniformMat4("uView", mView);
}

void Camera::bindProjectionMatrix(const Shader& shader) const
{
	shader.SetUniformMat4("uProjection", mProjection);
}

void Camera::updateViewMatrix()
{
	auto target = mPosition + mFront;
	mView = glm::lookAt(mPosition, target, mUp);
}

void Camera::updateProjectionMatrix(bool isPerspective /*= true */)
{
	if (isPerspective)
	{
		mProjection = glm::perspective(glm::radians(mFOV), mAspectRatio,
			mNearZ, mFarZ);
	}
	else
	{
		mProjection = glm::ortho(0.0f, mWidth, 0.0f, mHeight, mNearZ, mFarZ);
	}
}
