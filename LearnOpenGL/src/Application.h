#pragma once

#include <string_view>
#include <memory>
#include <glew/glew.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>


class Camera;


class Application
{
public:
	static Application& Get();

public:
	Application() = default;
	~Application();
	Application(const Application&) = delete;
	Application& operator=(const Application&) = delete;

	void Init(std::string_view title, int width, int height, int glVersionMajor,
		int glVersionMinor);

	void Run();

	void SetViewport(int width, int height);
	int GetScreenWidth() const;
	int GetScreenHeight() const;

	void Shutdown();
	void MoveCamera(int keycode);
	void TurnCamera(const glm::vec3& direction);
	void ZoomCamera(float offset);

private:
	void createWindowAndContext(std::string_view title, int width, int height,
		int glVersionMajor, int glVersionMinor);
	void initGLEW();
	void setCallbacks();
	void createCamera();

private:
	GLFWwindow* mWindow = nullptr;
	std::unique_ptr<Camera> mCamera;
	int mScreenWidth = 0;
	int mScreenHeight = 0;
	float mDeltaTime = 0.0f;
};

