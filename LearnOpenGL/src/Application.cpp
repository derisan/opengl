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
	glm::vec3 cubePositions[] = {
	glm::vec3(0.0f,  0.0f,  0.0f),
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	std::vector<std::unique_ptr<Cube>> cubes;
	cubes.reserve(std::size(cubePositions));

	for (auto i = 0; i < std::size(cubePositions); i++)
	{
		auto cube = std::make_unique<Cube>();
		cube->SetVertexArray(VertexArray::GetVertexArray("Cube"));
		cube->SetTexture(Texture::GetTexture("Assets/Images/container2.png"));
		cube->SetTexture(Texture::GetTexture("Assets/Images/container2_specular.png"));
		cube->SetTexture(Texture::GetTexture("Assets/Images/black.jpg"));
		cube->SetScale(glm::vec3{ 1.0f, 2.0f, 1.0f });
		cube->SetShininess(32.0f);
		cube->SetPosition(cubePositions[i]);
		cubes.push_back(std::move(cube));
	}

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

		defaultShader.Bind();
		
		setLightUniforms(defaultShader);
		mCamera->Bind(defaultShader);

		for (const auto& cube : cubes)
		{
			cube->Draw(defaultShader);
		}

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

void Application::setLightUniforms(const Shader& shader)
{
	// Viewer's position
	shader.SetUniform3f("uViewerPosition", mCamera->GetCameraPos());

	// Directional Light
	shader.SetUniform3f("uDirLight.Direction", glm::vec3{ -0.2f, -1.0f, -0.3f });
	shader.SetUniform3f("uDirLight.Ambient", glm::vec3{ 0.05f });
	shader.SetUniform3f("uDirLight.Diffuse", glm::vec3{ 0.4f });
	shader.SetUniform3f("uDirLight.Specular", glm::vec3{ 0.5f });

	// Spotlight
	shader.SetUniform3f("uSpotLight.Position", mCamera->GetCameraPos());
	shader.SetUniform3f("uSpotLight.Direction", mCamera->GetCameraFront());
	shader.SetUniform3f("uSpotLight.Ambient", glm::vec3{ 0.0f });
	shader.SetUniform3f("uSpotLight.Diffuse", glm::vec3{ 1.0f });
	shader.SetUniform3f("uSpotLight.Specular", glm::vec3{ 1.0f });
	shader.SetUniform1f("uSpotLight.Cutoff", glm::cos(glm::radians(12.5f)));
	shader.SetUniform1f("uSpotLight.OuterCutoff", glm::cos(glm::radians(15.0f)));
	shader.SetUniform1f("uSpotLight.Linear", 0.09f);
	shader.SetUniform1f("uSpotLight.Quadratic", 0.032f);

	// Point lights
	static glm::vec3 pointLightPositions[] = {
		glm::vec3(0.7f,  0.2f,  2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f,  2.0f, -12.0f),
		glm::vec3(0.0f,  0.0f, -3.0f)
	};

	for (auto i = 0; i < 4; i++)
	{
		shader.SetUniform3f("uPointLights[" + std::to_string(i) + "].Position", 
			pointLightPositions[i]);
		shader.SetUniform3f("uPointLights[" + std::to_string(i) + "].Ambient", 
			glm::vec3{ 0.05f });
		shader.SetUniform3f("uPointLights[" + std::to_string(i) + "].Diffuse", 
			glm::vec3{ 0.8f });
		shader.SetUniform3f("uPointLights[" + std::to_string(i) + "].Specular",
			glm::vec3{ 1.0f });
		shader.SetUniform1f("uPointLights[" + std::to_string(i) + "].Linear",
			0.09f);
		shader.SetUniform1f("uPointLights[" + std::to_string(i) + "].Quadratic",
			0.032f);
	}
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

