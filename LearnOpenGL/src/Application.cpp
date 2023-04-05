#include "Application.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <algorithm>

#include "Macros.h"
#include "Shader.h"
#include "VertexArray.h"
#include "Texture.h"
#include "Renderable.h"
#include "Camera.h"
#include "Cube.h"


void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void MouseCallback(GLFWwindow* window, double x, double y);
void MouseScrollCallback(GLFWwindow* window, double xOffset, double yOffset);
void WindowSizeCallback(GLFWwindow* window, int w, int h);

Application& Application::Get()
{
	static Application app;
	return app;
}

Application::~Application()
{
	Texture::Clear();
	VertexArray::Clear();
	glfwTerminate();
}

void Application::Init(std::string_view title, int width, int height, int glVersionMajor, int glVersionMinor)
{
	mScreenWidth = width;
	mScreenHeight = height;

	createWindowAndContext(title, width, height, glVersionMajor, glVersionMinor);
	setCallbacks();
	initGLEW();
	createCamera();
}

void Application::Run()
{
	auto cube = std::make_unique<Cube>();
	cube->SetVertexArray(VertexArray::GetVertexArray("Cube"));
	cube->SetColor(glm::vec3{ 1.0f, 0.5f, 0.31f });
	cube->SetScale(glm::vec3{ 1.0f, 2.0f, 1.0f });
	float cubeYRotation = 0.0f;

	auto lightCube = std::make_unique<LightCube>();
	lightCube->SetVertexArray(VertexArray::GetVertexArray("Cube"));
	glm::vec3 lightCubePos{ 1.2f, 1.0f, 2.0f };
	lightCube->SetPosition(lightCubePos);
	lightCube->SetScale(glm::vec3{ 0.2f });

	Shader defaultShader{ "Assets/Shaders/DefaultVS.glsl", "Assets/Shaders/DefaultFS.glsl" };
	Shader lightCubeShader{ "Assets/Shaders/DefaultVS.glsl", "Assets/Shaders/LightCubeFS.glsl" };

	GLCall(glEnable(GL_DEPTH_TEST));

	float lastFrame = static_cast<float>(glfwGetTime());
	float currentFrame = lastFrame;

	while (NOT glfwWindowShouldClose(mWindow))
	{
		currentFrame = static_cast<float>(glfwGetTime());
		mDeltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		// Rotate 
		lightCubePos.x = cos(static_cast<float>(glfwGetTime())) * 2.0f;
		lightCubePos.z = sin(static_cast<float>(glfwGetTime())) * 2.0f;
		lightCube->SetPosition(lightCubePos);
		cubeYRotation += 30.0f * mDeltaTime;
		cube->SetRotation(glm::vec3{ 0.0f, cubeYRotation, 0.0f });

		defaultShader.Bind();
		defaultShader.SetUniform3f("uLightColor", glm::vec3{ 1.0f });
		defaultShader.SetUniform3f("uLightPosition", lightCubePos);
		defaultShader.SetUniform3f("uViewerPosition", mCamera->GetCameraPos());
		defaultShader.SetUniform1f("uAmbientStrength", 0.1f);
		defaultShader.SetUniform1f("uSpecularStrength", 0.5f);
		mCamera->Bind(defaultShader);
		cube->Draw(defaultShader);

		lightCubeShader.Bind();
		mCamera->Bind(lightCubeShader);
		lightCube->Draw(lightCubeShader);

		glfwSwapBuffers(mWindow);
		glfwPollEvents();
	}
}

void Application::SetViewport(int width, int height)
{
	mScreenWidth = width;
	mScreenHeight = height;
	glViewport(0, 0, width, height);
}

int Application::GetScreenWidth() const
{
	return mScreenWidth;
}

int Application::GetScreenHeight() const
{
	return mScreenHeight;
}

void Application::Shutdown()
{
	glfwSetWindowShouldClose(mWindow, true);
}

void Application::MoveCamera(int keycode)
{
	const float cameraSpeed = 50.0f * mDeltaTime;

	auto cameraPos = mCamera->GetCameraPos();

	switch (keycode)
	{
	case GLFW_KEY_W:
		cameraPos += mCamera->GetCameraFront() * cameraSpeed;
		break;
	case GLFW_KEY_S:
		cameraPos -= mCamera->GetCameraFront() * cameraSpeed;
		break;
	case GLFW_KEY_A:
		cameraPos -= glm::cross(mCamera->GetCameraFront(), mCamera->GetCameraUp())
			* cameraSpeed;
		break;
	case GLFW_KEY_D:
		cameraPos += glm::cross(mCamera->GetCameraFront(), mCamera->GetCameraUp())
			* cameraSpeed;
		break;
	case GLFW_KEY_Q:
		cameraPos += mCamera->GetCameraUp() * cameraSpeed;
		break;
	case GLFW_KEY_E:
		cameraPos -= mCamera->GetCameraUp() * cameraSpeed;
		break;
	default:
		ASSERT(false);
		break;
	}

	mCamera->SetCameraPos(cameraPos);
}

void Application::TurnCamera(const glm::vec3& direction)
{
	mCamera->SetCameraFront(direction);
}

void Application::ZoomCamera(float offset)
{
	auto fov = mCamera->GetFOV();

	// Scroll Up -> offset = 1
	// Scroll Down -> offset = -1
	fov -= offset;

	fov = std::clamp(fov, 1.0f, 60.0f);
	mCamera->SetFOV(fov);
}

void Application::createWindowAndContext(std::string_view title, int width, int height, int glVersionMajor, int glVersionMinor)
{
	if (NOT glfwInit())
	{
		ASSERT(false);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, glVersionMajor);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, glVersionMinor);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	mWindow = glfwCreateWindow(width, height, title.data(), nullptr, nullptr);

	if (NOT mWindow)
	{
		ASSERT(false);
	}

	// 마우스 가두기
	glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwMakeContextCurrent(mWindow);
}

void Application::initGLEW()
{
	if (auto err = glewInit(); GLEW_OK != err)
	{
		std::cout << glewGetErrorString(err) << std::endl;
		ASSERT(false);
	}

	std::cout << glGetString(GL_VERSION) << std::endl;
}

void Application::setCallbacks()
{
	glfwSetKeyCallback(mWindow, KeyCallback);
	glfwSetCursorPosCallback(mWindow, MouseCallback);
	glfwSetFramebufferSizeCallback(mWindow, WindowSizeCallback);
	glfwSetScrollCallback(mWindow, MouseScrollCallback);
}

void Application::createCamera()
{
	mCamera = std::make_unique<Camera>();
	mCamera->SetViewMatrix(glm::vec3{ 0.0f, 0.0f, 5.0f },
		glm::vec3{ 0.0f, 0.0f, -1.0f },
		glm::vec3{ 0.0f, 1.0f, 0.0f });
	mCamera->SetPerspectiveMatrix(45.0f,
		mScreenWidth / static_cast<float>(mScreenHeight),
		0.1f, 100.0f);
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	auto& app = Application::Get();

	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
	{
		app.Shutdown();
	}

	if ((GLFW_KEY_W == key ||
		GLFW_KEY_S == key ||
		GLFW_KEY_A == key ||
		GLFW_KEY_D == key || 
		GLFW_KEY_Q == key ||
		GLFW_KEY_E == key)
		&& (GLFW_PRESS == action ||
			GLFW_REPEAT == action))
	{
		app.MoveCamera(key);
	}
}


void MouseCallback(GLFWwindow* window, double x, double y)
{
	static bool bFirstFocus = true;
	static float lastX = Application::Get().GetScreenWidth() / 2.0f;
	static float lastY = Application::Get().GetScreenHeight() / 2.0f;

	if (bFirstFocus)
	{
		lastX = static_cast<float>(x);
		lastY = static_cast<float>(y);
		bFirstFocus = false;
	}

	float xOffset = static_cast<float>(x - lastX);
	float yOffset = static_cast<float>(lastY - y);

	lastX = static_cast<float>(x);
	lastY = static_cast<float>(y);

	static float sensitivity = 0.1f;
	xOffset *= sensitivity;
	yOffset *= sensitivity;

	static float yaw = -90.0f;
	static float pitch = 0.0f;

	yaw += xOffset;
	pitch += yOffset;

	pitch = std::clamp(pitch, -89.0f, 89.0f);

	glm::vec3 direction = glm::vec3{ 0.0f };
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	Application::Get().TurnCamera(direction);
}


void MouseScrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
	Application::Get().ZoomCamera(static_cast<float>(yOffset));
}


void WindowSizeCallback(GLFWwindow* window, int w, int h)
{
	auto& app = Application::Get();
	app.SetViewport(w, h);
}

